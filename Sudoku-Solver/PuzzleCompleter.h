//
//  PuzzleCompleter.h
//  Sudoku-Solver
//
//  Created by Mickey on 12/29/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#ifndef Sudoku_Solver_PuzzleCompleter_h
#define Sudoku_Solver_PuzzleCompleter_h

#include "opencv2/core/core.hpp"

class PuzzleCompleter {
    
private:
    
    
    
public:
    
    PuzzleCompleter() {
        
    }
    
    void complete( const Mat& undistorted , vector<vector<int>> unsolved , vector<vector<int>> solved ) {
        ImageDisplay disp;
        disp.showImage( "Original" , undistorted );
        
        int cellSideLength = ceil(static_cast<double>(undistorted.size().width)/9.0);
        for ( int row=0 ; row<9 ; ++row ) {
            for ( int col=0 ; col<9 ; ++col ) {
                if ( unsolved[ row ][ col ] != -1 ) {
                    continue;
                }
                ostringstream digit;
                digit << solved[ row ][ col ];
                
                int baseline;
                Size textSize = getTextSize( digit.str() , CV_FONT_HERSHEY_PLAIN , 2.0f , 3 , &baseline );
                
                int startX = col*cellSideLength + cellSideLength/2 - textSize.width/2;
                int startY = row*cellSideLength + cellSideLength/2 + textSize.height/2;
                
                putText( undistorted , digit.str() , Point( startX , startY ) ,
                        CV_FONT_HERSHEY_PLAIN , 2.0f , CV_RGB( 255 , 0 , 0 ) , 3 );
            }
        }//*/
    }
    
    
};

#endif
