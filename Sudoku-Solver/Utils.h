//
//  Utils.h
//  Sudoku-Solver
//
//  Created by Mickey on 12/25/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#ifndef Sudoku_Solver_Utils_h
#define Sudoku_Solver_Utils_h

#include <string>
using std::string;

#include "opencv2/core/core.hpp"
using cv::Mat;

#include "opencv2/highgui/highgui.hpp"
using cv::imshow;
using cv::waitKey;
using cv::destroyWindow;

class ImageDisplay {
    
private:
    bool _on;
    bool _destroyAfterShow;
    
    
public:
    ImageDisplay( bool on = true , bool destroyAfterShow = true ) :
          _on( on )
        , _destroyAfterShow( destroyAfterShow ) {
    }
    
    void enable() {
        _on = true;
    }
    
    void disable() {
        _on = false;
    }
    
    void setDestroyAfterShowing( bool value ) {
        _destroyAfterShow = value;
    }
    
    void showImage( const string& windowName , const Mat& im ) const {
        if ( _on ) {
            imshow( windowName , im );
            waitKey( 0 );
            if ( _destroyAfterShow ) {
                destroyWindow( windowName );
            }
        }
    }
};

#endif
