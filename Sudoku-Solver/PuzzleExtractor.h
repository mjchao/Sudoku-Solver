//
//  GridExtractor.h
//  Sudoku-Solver
//
//  Created by Mickey on 12/12/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#ifndef Sudoku_Solver_GridExtractor_h
#define Sudoku_Solver_GridExtractor_h

#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>
using std::max;

#include <cmath>
using std::sqrt;

#include "Utils.h"

#include "opencv2/opencv.hpp"
using namespace cv;

/**
 * Extracts the 9x9 grid from a Sudoku puzzle
 */
class PuzzleExtractor {
    
private:
    const Mat _puzzle;
    ImageDisplay _disp;
    
    void invertImage( const Mat& image , Mat& destImage ) {
        
        //blur the image to make it easier to extract gridlines
        Mat blurred = Mat( image.size() , CV_8UC1 );
        GaussianBlur( image , blurred , Size(11,11) , 0 );
        _disp.showImage( "Blurred Puzzle" , blurred );
        
        //Turn the image into black-and-white by setting a threshold
        //value at which we set a pixel as black or white
        destImage = Mat( _puzzle.size() , CV_8UC1 );
        adaptiveThreshold( blurred , destImage , 255 , ADAPTIVE_THRESH_MEAN_C , THRESH_BINARY , 5 , 2 );
        _disp.showImage( "Outer Box" , destImage );
        
        //invert the image so that everything that is not whitespace
        //becomes white and all the whitespace becomes black
        bitwise_not( destImage , destImage );
        _disp.showImage( "Inverted Outer Box" , destImage );
    }
    
    void drawLine( Vec2f& line , Mat& img , Scalar rgb = CV_RGB( 0 , 0 , 255 ) , int thickness = 1 ) {
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
        cv::line( img , start , end , rgb , thickness );
    }

    void mergeRelatedLines( vector<Vec2f>* lines , Mat& img ) {
        
        //check lines pair by pair and merge them together if they are close
        //enough together. We indicate a line has been merged by setting
        //its r value to 0 and theta to -100.
        for ( auto current = lines->begin() ; current != lines->end() ; ++current ) {
            if ( (*current)[ 0 ] == 0 && (*current)[ 1 ] == -100 ) {
                continue;
            }
            
            float rCurrent = (*current)[ 0 ];
            float thetaCurrent = (*current)[ 1 ];
            
            //find 2 endpoints of the line.
            //if the line is more vertical than it is horizontal, then
            //we pick the endpoints using x=0 and x=width
            //if the line is more horizontal than vertical, then we pick
            //the endpoints using y=0 and y=height
            //this ensures that both dimensions of the image are completely
            //covered in this range
            Point p1Current;
            Point p2Current;
            if ( thetaCurrent > CV_PI/180*45 && thetaCurrent < CV_PI/180*135 ) {
                p1Current.x = 0;
                p1Current.y = rCurrent/sin(thetaCurrent);
                p2Current.x = img.size().width;
                p2Current.y = -p2Current.x/tan(thetaCurrent) + rCurrent/sin(thetaCurrent);
            }
            else {
                p1Current.y = 0;
                p1Current.x = rCurrent/cos(thetaCurrent);
                p2Current.y = img.size().height;
                p2Current.x = -p2Current.y/tan(thetaCurrent) + rCurrent/cos(thetaCurrent);
            }
            
            for ( auto next = current+1 ; next <= lines->end() ; ++next ) {
                
                //check if the two lines are close together - which means
                //their r values are within 20 units of each other and
                //theta values are within 10 degrees of each other.
                if ( fabs((*next)[0] - (*current)[0]) < 20 &&
                    fabs((*next)[1] - (*current)[1]) < CV_PI/180*10 ) {
                    
                    float rNext = (*next)[0];
                    float thetaNext = (*next)[1];
                    
                    //find 2 endpoints of the next line
                    Point p1Next;
                    Point p2Next;
                    if ( thetaNext > CV_PI/180*45 && thetaNext < CV_PI/180*135 ) {
                        p1Next.x = 0;
                        p1Next.y = rNext/sin(thetaNext);
                        p2Next.x = img.size().width;
                        p2Next.y = -p2Next.x/tan(thetaNext) + rNext/sin(thetaNext);
                    }
                    else {
                        p1Next.y = 0;
                        p1Next.x = rNext/cos(thetaNext);
                        p2Next.y = img.size().height;
                        p2Next.x = -p2Next.y/tan(thetaNext) + rNext/cos(thetaNext);
                    }
                    
                    //merge the two lines together only if their endpoints
                    //are close together as well
                    double dx1 = p1Next.x - p1Current.x;
                    double dy1 = p1Next.y - p1Current.y;
                    double dx2 = p2Next.x - p2Current.x;
                    double dy2 = p2Next.y - p2Current.y;
                    if ( dx1*dx1 + dy1*dy1 < 64*64 && dx2*dx2 + dy2*dy2 < 64*64 ) {
                        (*current)[ 0 ] = ((*current)[0] + (*next)[0])/2;
                        (*current)[ 1 ] = ((*current)[1] + (*next)[1])/2;
                        (*next)[0] = 0;
                        (*next)[1] = -100;
                    }
                }
            }
        }
    }
    
public:
    
    PuzzleExtractor( const Mat& puzzle ) : _puzzle( puzzle ) {
        _disp.disable();
    }
    
    Mat extractPuzzle() {
        Mat outerBox = Mat( _puzzle.size() , CV_8UC1 );
        invertImage( _puzzle , outerBox );
        
        //dilate the image so that cracks between lines get filled in
        Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,0,1,1,0,1,0 );
        dilate( outerBox , outerBox , kernel );
        _disp.showImage( "Dilated and inverted outer box" , outerBox );
        
        //find a point that is part of the largest blob of white. We assume
        //that the Sudoku Puzzle is going to be the largest piece of white
        //in the image and that other stuff is noise.
        int maxArea = 0;
        Point maxPoint;
        for ( int y=0 ; y<outerBox.size().height ; ++y ) {
            uchar* row = outerBox.ptr(y);
            for ( int x=0 ; x<outerBox.size().width ; ++x ) {
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
        for ( int y=0 ; y<outerBox.size().height ; ++y ) {
            uchar* row = outerBox.ptr( y );
            for ( int x=0 ; x<outerBox.size().width ; ++x ) {
                if ( row[ x ] == 64 && x != maxPoint.x && y != maxPoint.y ) {
                    floodFill( outerBox , Point(x,y) , CV_RGB( 0 , 0 , 0 ) );
                }
            }
        }
        _disp.showImage( "Thresholded but dilated outer box" , outerBox );
        
        erode( outerBox , outerBox , kernel );
        _disp.showImage( "Thresholded and restored outer box" , outerBox );
        
        //find the lines that make the outer (thick) Sudoku grid
        vector< Vec2f > lines;
        HoughLines( outerBox , lines , 1 , CV_PI/180 , 200 );
        mergeRelatedLines( &lines , outerBox );
        for ( Vec2f line : lines ) {
            drawLine( line , outerBox , CV_RGB( 0 , 0 , 128 ) );
        }
        _disp.showImage( "Potential Grid Lines" , outerBox );
        
        //find the outer boundaries of the sudoku puzzle
        Vec2f topEdge( 1000 , 1000 );
        
        Vec2f bottomEdge( -1000 , -1000 );
        
        Vec2f leftEdge( 1000 , 1000 );
        double leftXIntercept = 10000;
        
        Vec2f rightEdge( -1000 , -1000 );
        double rightXIntercept = 0;

        for ( const Vec2f& line : lines ) {
            float r = line[ 0 ];
            float theta = line[ 1 ];
            
            //merged lines can be ignored
            if ( r == 0 && theta == -1000 ) {
                continue;
            }
            
            //look for vertical lines
            if ( theta > CV_PI/180*80 && theta < CV_PI/180*100 ) {
                if ( r < topEdge[ 0 ] ) {
                    topEdge = line;
                }
                if ( r > bottomEdge[ 0 ] ) {
                    bottomEdge = line;
                }
                
            }
            
            else if ( theta > CV_PI/180*10/180 || theta > CV_PI/180*170 ) {
                double xIntercept = r/cos( theta );
                if ( xIntercept > rightXIntercept ) {
                    rightEdge = line;
                    rightXIntercept = xIntercept;
                }
                else if ( xIntercept <= leftXIntercept ) {
                    leftEdge = line;
                    leftXIntercept = xIntercept;
                }
            }
        }
        
        Mat puzzleCopy;
        cvtColor( _puzzle , puzzleCopy , CV_GRAY2RGB );
        drawLine( topEdge , puzzleCopy , CV_RGB( 255 , 0 , 0 ) , 2 );
        drawLine( bottomEdge , puzzleCopy , CV_RGB( 255 , 0 , 0 ) , 2 );
        drawLine( leftEdge , puzzleCopy , CV_RGB( 255 , 0 , 0 ) , 2 );
        drawLine( rightEdge , puzzleCopy , CV_RGB( 255 , 0 , 0 ) , 2 );
        _disp.showImage( "Outer Grid Lines" , puzzleCopy );
        
        //now we need to undistort the image
        //first find two points on each of the grid lines, so that
        //we can compute the intersections of the grid lines
        Point2f left1 , left2;
        if ( leftEdge[ 1 ] != 0 ) {
            left1.x = 0;
            left1.y = leftEdge[ 0 ]/sin( leftEdge[ 1 ] );
            left2.x = puzzleCopy.size().width;
            left2.y = -left2.x / tan(leftEdge[ 1 ] ) + left1.y;
        }
        else {
            left1.y = 0;
            left1.x = leftEdge[ 0 ]/cos( leftEdge[ 1 ] );
            left2.y = puzzleCopy.size().height;
            left2.x = left1.x - puzzleCopy.size().height*tan(leftEdge[ 1 ] );
        }
        
        Point2f right1 , right2;
        if ( rightEdge[ 1 ] != 0 ) {
            right1.x = 0;
            right1.y = rightEdge[ 0 ]/sin( rightEdge[ 1 ] );
            right2.x = puzzleCopy.size().width;
            right2.y = -right2.x / tan( rightEdge[ 1 ] ) + right1.y;
        }
        else {
            right1.y = 0;
            right1.x = rightEdge[ 0 ]/cos( rightEdge[ 1 ] );
            right2.y = puzzleCopy.size().height;
            right2.x = right2.x - puzzleCopy.size().height*tan( rightEdge[ 1 ] );
        }
        
        Point2f top1 , top2;
        top1.x = 0;
        top1.y = topEdge[ 0 ] / sin( topEdge[ 1 ] );
        top2.x = puzzleCopy.size().width;
        top2.y = -top2.x / tan( topEdge[ 1 ] ) + top1.y;
        
        
        Point2f bottom1 , bottom2;
        bottom1.x = 0;
        bottom1.y = bottomEdge[ 0 ] / sin( bottomEdge[ 1 ] );
        bottom2.x = puzzleCopy.size().width;
        bottom2.y = -bottom2.x / tan( bottomEdge[ 1 ] ) + bottom1.y;
        
        //compute the intersections of the grid lines
        //top and left:
        Point2f dTop = top1 - top2;
        Point2f dLeft = left1 - left2;
        float u = (left1 - top1).cross( dTop ) / (dTop.cross(dLeft));
        Point2f topLeftIntersection = left1 + u*dLeft;
        
        //top and right:
        Point2f dRight = right1 - right2;
        u = (right1 - top1).cross( dTop ) / (dTop.cross(dRight));
        Point2f topRightIntersection = right1 + u*dRight;
        
        //bottom and left:
        Point2f dBottom = bottom1 - bottom2;
        u = (left1 - bottom1).cross( dBottom ) / (dBottom.cross(dLeft));
        Point2f bottomLeftIntersection = left1 + u*dLeft;
        
        //bottom and right:
        u = (right1 - bottom1).cross( dBottom ) / (dBottom.cross(dRight));
        Point2f bottomRightIntersection = right1 + u*dRight;
        
        cv::circle( puzzleCopy , topLeftIntersection , 10 , CV_RGB( 0 , 0 , 250 ) , -1 );
        cv::circle( puzzleCopy , topRightIntersection , 10 , CV_RGB( 0 , 0 , 250 ) , -1 );
        cv::circle( puzzleCopy , bottomLeftIntersection , 10 , CV_RGB( 0 , 0 , 250 ) , -1 );
        cv::circle( puzzleCopy , bottomRightIntersection , 10 , CV_RGB( 0 , 0 , 250 ) , -1 );
        _disp.showImage( "Puzzle Corners" , puzzleCopy );
        
        //calculate the maximum edge length - we will then try to undistort the
        //original image so that it is a square with that side length
        Point2f topLength = topLeftIntersection - topRightIntersection;
        float maxLength = topLength.dot( topLength );
        Point2f bottomLength = bottomLeftIntersection - bottomRightIntersection;
        maxLength = max( maxLength , bottomLength.dot( bottomLength ) );
        Point2f leftLength = topLeftIntersection - bottomLeftIntersection;
        maxLength = max( maxLength , leftLength.dot( leftLength ) );
        Point2f rightLength = topRightIntersection - bottomRightIntersection;
        maxLength = max( maxLength , rightLength.dot( rightLength ) );
        maxLength = sqrt( maxLength );
        
        Point2f src[4] , dst[4];
        src[ 0 ] = topLeftIntersection;
        dst[ 0 ] = Point2f( 0 , 0 );
        src[ 1 ] = topRightIntersection;
        dst[ 1 ] = Point2f( maxLength-1 , 0 );
        src[ 2 ] = bottomRightIntersection;
        dst[ 2 ] = Point2f( maxLength-1 , maxLength-1 );
        src[ 3 ] = bottomLeftIntersection;
        dst[ 3 ] = Point2f( 0 , maxLength-1 );
        Mat undistorted = Mat(Size(maxLength , maxLength) , CV_8UC1 );
        cv::warpPerspective(_puzzle, undistorted ,
            cv::getPerspectiveTransform( src , dst ) , Size( maxLength , maxLength ) );
        
        _disp.showImage( "Undistorted Puzzle" , undistorted );
        return undistorted;
    }
};

#endif
