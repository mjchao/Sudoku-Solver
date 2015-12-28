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
    
public:
    
    PuzzleReader( const Mat& puzzle , const DigitRecognizer& digitRecognizer ) :
        _puzzle( puzzle )
        , _digitRecognizer( digitRecognizer ) {
        
    }
    
    void getDigits( vector<vector<int>>& digits ) {
        _disp.setDestroyAfterShowing( false );
        digits.resize( 9 , vector<int>(9) );
        
        _disp.showImage( "Undistorted Puzzle" , _puzzle );
        
        Mat invertedPuzzle = _puzzle.clone();
        adaptiveThreshold( _puzzle , invertedPuzzle , 255 ,
            CV_ADAPTIVE_THRESH_GAUSSIAN_C , CV_THRESH_BINARY_INV , 101 , 1 );
        _disp.showImage( "Inverted Puzzle" , invertedPuzzle );
        
        int cellSideLength = ceil(static_cast<double>(invertedPuzzle.size().width)/9.0);
        Mat currentCell = Mat( cellSideLength , cellSideLength , CV_8UC1 );
        
        uchar* ptr = currentCell.ptr( 0 );
        for ( int r=0 ; r<cellSideLength ; ++r ) {
            for ( int c=0 ; c<cellSideLength ; ++c ) {
                ptr[ r*cellSideLength + c ] = invertedPuzzle.at<uchar>( r , 5*cellSideLength+c );
            }
        }
        Mat resizedCell = Mat( 28 , 28 , CV_8UC1 );
        cv::resize( currentCell , resizedCell , resizedCell.size() );
        _disp.showImage( "Cell (0,0)" , resizedCell );
        
        Mat reshapedCell = Mat( 1 , 28*28 , CV_32F );
        for ( int r=0 ; r<28 ; ++r ) {
            for ( int c=0 ; c<28 ; ++c ) {
                reshapedCell.at<float>( 0 , r*28+c ) = static_cast<float>(resizedCell.at<uchar>(r , c)) / 255.0f;
            }
        }
        
        cout << _digitRecognizer.predict( reshapedCell ) << endl;
    }
    
};

#endif
