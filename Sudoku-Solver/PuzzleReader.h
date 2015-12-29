//
//  PuzzleReader.h
//  Sudoku-Solver
//
//  Created by Mickey on 12/26/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#ifndef Sudoku_Solver_PuzzleReader_h
#define Sudoku_Solver_PuzzleReader_h

#include <vector>
using std::vector;

#include "DigitRecognizer.h"
#include "Utils.h"

#include "opencv2/core/core.hpp"

/**
 * Reads the numbers and blanks from a 9x9 sudoku puzzle that
 * has already been extracted from an image by the PuzzleExtractor
 */
class PuzzleReader {
    
private:
    
    Mat _puzzle;
    const DigitRecognizer& _digitRecognizer;
    ImageDisplay _disp;
    
    int countWhitePixelsInCenter( const Mat& cell ) {
        int count = 0;
        for ( int r=cell.size().height/2-2 ; r<cell.size().height/2+2 ; ++r ) {
            for ( int c=cell.size().width/2-2 ; c<cell.size().width+2 ; ++c ) {
                if ( static_cast<int>( cell.at<uchar>(r , c) ) == 255 ) {
                    ++count;
                }
            }
        }
        return count;
    }
    
    void getWhiteBoundaries( const Mat& cell , int& leftX , int& rightX , int& topY , int& bottomY) {
        for ( int r=0 ; r<cell.size().height ; ++r ) {
            for ( int c=0 ; c<cell.size().width ; ++c ) {
                if ( static_cast<int>( cell.at<uchar>( r , c ) ) == 255 ) {
                    leftX = min( leftX , c );
                    rightX = max( rightX , c );
                    topY = min( r , topY );
                    bottomY = max( r , bottomY );
                }
            }
        }
    }
    
    int getDigit( const Mat& cell ) {
        
        //first shrink the digit so that it fits in a 20x20 image
        Mat shrunkenCell = Mat( 20 , 20 , CV_8UC1 );
        cv::resize( cell , shrunkenCell , shrunkenCell.size() );
        
        int borderSize = 4;
        //then turn everything into black and white
        for ( int r=0 ; r<20 ; ++r ) {
            for ( int c=0 ; c<20 ; ++c ) {
                
                //don't penalize the top edge because it tends to
                //be accurate - but the left, right, and bottom edges
                //tend to have extraneous gridlines
                if ( r+borderSize < 0 || r > 20-borderSize || c < borderSize || c > 20 - borderSize ) {
                    shrunkenCell.at<uchar>( r , c ) = 0;
                }
                else {
                    shrunkenCell.at<uchar>( r , c ) = shrunkenCell.at<uchar>( r , c ) >= 128 ? 255 : 0;
                }
            }
        }
        
        //if there aren't enough white pixels, assume it is an empty square
        int whitePixels = countWhitePixelsInCenter( shrunkenCell );
        if ( whitePixels < 8 ) {
            return -1;
        }
        /*
        // DEBUG
        else {
            ostringstream title;
            title << "This cell has " << whitePixels << " white pixels";
            _disp.enable();
            _disp.showImage( title.str() , shrunkenCell );
            _disp.disable();
        }//*/
        
        //only keep the biggest blob of white - everything else should be
        //noise and turned to black
        int maxArea = -1;
        Point maxPoint;
        for ( int r=0 ; r<20 ; ++r ) {
            uchar* row = shrunkenCell.ptr( r );
            for ( int c=0 ; c<20 ; ++c ) {
                if ( row[ c ] == 255 ) {
                    int area = floodFill( shrunkenCell , Point( c , r ) , CV_RGB( 0 , 0 , 64 ) );
                    if ( area > maxArea ) {
                        maxArea = area;
                        maxPoint = Point( c , r );
                    }
                }
            }
        }
        
        //keep biggest area white
        floodFill( shrunkenCell , maxPoint , CV_RGB( 255 , 255 , 255 ) );
        
        //keep all other areas black
        for ( int r=0 ; r<20 ; ++r ) {
            uchar* row = shrunkenCell.ptr( r );
            for ( int c=0 ; c<20 ; ++c ) {
                if ( row[ c ] == 64 && c != maxPoint.x && r != maxPoint.y ) {
                    floodFill( shrunkenCell , Point( c , r ) , CV_RGB( 0 , 0 , 0 ) );
                }
            }
        }
        
        //then we will try to center the digit
        //first, find the boundaries of the white pixels
        int leftX = maxPoint.x;
        int rightX = maxPoint.x;
        int topY = maxPoint.y;
        int bottomY = maxPoint.y;
        getWhiteBoundaries( shrunkenCell , leftX , rightX , topY , bottomY );
        
        int width = rightX - leftX + 1;
        int height = bottomY - topY + 1;
        Mat isolatedDigit = shrunkenCell( Rect( leftX , topY , width , height ) );
        _disp.showImage( "Isolated Digit" , isolatedDigit );
        
        Mat paddedDigit = Mat::zeros( 28 , 28 , CV_8UC1 );
        Rect copyLoc( (28-width)/2 , (28-height)/2 , width , height );
        isolatedDigit.copyTo( paddedDigit(copyLoc) );
        
        Mat reshapedDigit = Mat( 1 , 28*28 , CV_32F );
        for ( int r=0 ; r<28 ; ++r ) {
            for ( int c=0 ; c<28 ; ++c ) {
                reshapedDigit.at<float>( 0 , r*28 + c ) =
                static_cast<float>( paddedDigit.at<uchar>( r , c ) ) / 255.0f;
            }
        }
        return _digitRecognizer.predict( reshapedDigit );
    }
    
public:
    
    PuzzleReader( const Mat& puzzle , const DigitRecognizer& digitRecognizer ) :
        _puzzle( puzzle )
        , _digitRecognizer( digitRecognizer ) {
        _disp.disable();
    }
    
    /**
     * Gets the digits in the Sudoku puzzle. If dilateFirst is set to true,
     * then the image will be dilated before we try to read the digits. This
     * should be used if the font of the puzzle is not thick enough.
     */
    void getDigits( vector<vector<int>>& digits , bool dilateFirst ) {
        _disp.setDestroyAfterShowing( false );
        digits.resize( 9 , vector<int>(9) );
        
        _disp.showImage( "Undistorted Puzzle" , _puzzle );
        
        Mat invertedPuzzle = _puzzle.clone();
        adaptiveThreshold( _puzzle , invertedPuzzle , 255 ,
            CV_ADAPTIVE_THRESH_GAUSSIAN_C , CV_THRESH_BINARY_INV , 101 , 1 );
        
        if ( dilateFirst ) {
            Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,0,1,1,0,1,0 );
            dilate( invertedPuzzle , invertedPuzzle , kernel );
        }
        
        _disp.showImage( "Inverted Puzzle" , invertedPuzzle );
        
        int cellSideLength = ceil(static_cast<double>(invertedPuzzle.size().width)/9.0);
        Mat currentCell = Mat( cellSideLength , cellSideLength , CV_8UC1 );
        
        for ( int row=0 ; row<9 ; ++row ) {
            uchar* ptr = currentCell.ptr( 0 );
            for( int col=0 ; col<9 ; ++col ) {
                for ( int r=0 ; r<cellSideLength ; ++r ) {
                    for ( int c=0 ; c<cellSideLength ; ++c ) {
                        ptr[ r*cellSideLength + c ] =
                            invertedPuzzle.at<uchar>( row*cellSideLength + r , col*cellSideLength + c );
                    }
                }
                int digit = getDigit( currentCell );
                digits[ row ][ col ] = digit;
            }
        }
    }
    
};

#endif
