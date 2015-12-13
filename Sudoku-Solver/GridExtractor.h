//
//  GridExtractor.h
//  Sudoku-Solver
//
//  Created by Mickey on 12/12/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#ifndef Sudoku_Solver_GridExtractor_h
#define Sudoku_Solver_GridExtractor_h

#include "opencv2/opencv.hpp"
using namespace cv;

/**
 * Extracts the grid from a Sudoku puzzle
 */
class GridExtractor {
    
private:
    const Mat _puzzle;
    
    void invertImage( const Mat& image , Mat& destImage ) {
        
        //blur the image to make it easier to extract gridlines
        Mat blurred = Mat( image.size() , CV_8UC1 );
        GaussianBlur( image , blurred , Size(11,11) , 0 );
        imshow( "Blurred Puzzle" , blurred );
        waitKey( 0 );
        
        //Turn the image into black-and-white by setting a threshold
        //value at which we set a pixel as black or white
        destImage = Mat( _puzzle.size() , CV_8UC1 );
        adaptiveThreshold( blurred , destImage , 255 , ADAPTIVE_THRESH_MEAN_C , THRESH_BINARY , 5 , 2 );
        imshow( "Outer Box" , destImage );
        waitKey( 0 );
        
        //invert the image so that everything that is not whitespace
        //becomes white and all the whitespace becomes black
        bitwise_not( destImage , destImage );
        imshow( "Inverted Outer Box" , destImage );
        waitKey( 0 );
    }
    
    void drawLine( Vec2f& line , Mat& img , Scalar rgb = CV_RGB( 0 , 0 , 255 ) ) {
        Point start;
        Point end;
        if ( line[ 1 ] != 0 ) {
            float slope = -1 / tan( line[ 1 ] );
            float intercept = line[ 0 ]/sin( line[ 1 ] );
            start = Point( 0 , intercept );
            end = Point( img.size().width , slope*img.size().width + intercept );
        }
        else {
            start = Point( line[ 0 ] , 0 );
            end = Point( line[ 0 ] , img.size().height );
        }
        cv::line( img , start , end , rgb );
    }
    
public:
    
    GridExtractor( const Mat& puzzle ) : _puzzle( puzzle ) {
        
    }
    
    void extractGrid( vector< Vec2f >& lines ) {
        Mat outerBox = Mat( _puzzle.size() , CV_8UC1 );
        invertImage( _puzzle , outerBox );
        
        //dilate the image so that cracks between lines get filled in
        Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,0,1,1,0,1,0 );
        dilate( outerBox , outerBox , kernel );
        imshow( "Dilated and inverted outer box" , outerBox );
        waitKey( 0 );
        
        //find a point that is part of the largest blob of white. We assume
        //that the Sudoku Puzzle is going to be the largest piece of white
        //in the image and that other stuff is noise.
        int maxArea = 0;
        Point maxPoint;
        for ( int y=0 ; y<=outerBox.size().height ; ++y ) {
            uchar* row = outerBox.ptr(y);
            for ( int x=0 ; x<=outerBox.size().width ; ++x ) {
                if ( row[ x ] >= 128 ) {
                    int area = floodFill( outerBox , Point(x,y) , CV_RGB( 0 , 0 , 64 ) );
                    if ( area > maxArea ) {
                        maxArea = area;
                        maxPoint = Point(x,y);
                    }
                }
            }
        }
        
        //turn the biggest blob white
        floodFill( outerBox , maxPoint , CV_RGB( 255 , 255 , 255 ) );
        
        //turn all other blobs to black
        for ( int y=0 ; y<=outerBox.size().height ; ++y ) {
            uchar* row = outerBox.ptr( y );
            for ( int x=0 ; x<=outerBox.size().width ; ++x ) {
                if ( row[ x ] == 64 && x != maxPoint.x && y != maxPoint.y ) {
                    floodFill( outerBox , Point(x,y) , CV_RGB( 0 , 0 , 0 ) );
                }
            }
        }
        imshow( "Thresholded but dilated outer box" , outerBox );
        waitKey( 0 );
        
        erode( outerBox , outerBox , kernel );
        imshow( "Thresholded and restored outer box" , outerBox );
        waitKey( 0 );
        
        //find the lines that make the outer (thick) Sudoku grid
        HoughLines( outerBox , lines , 1 , CV_PI/180 , 200 );
        for ( Vec2f line : lines ) {
            drawLine( line , outerBox , CV_RGB( 0 , 0 , 128 ) );
        }
        imshow( "Potential Grid Lines" , outerBox );
        waitKey( 0 );
    }
};

#endif
