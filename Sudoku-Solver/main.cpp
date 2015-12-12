//
//  main.cpp
//  Sudoku-Solver
//
//  Created by Mickey on 12/12/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "opencv2/opencv.hpp"
using namespace cv;

int main(int argc, const char * argv[]) {
    
    Mat image;
    
    string imageFilename;
    cin >> imageFilename;
    image = imread( imageFilename.c_str() , 1 );
    
    namedWindow( "Display Image" , WINDOW_AUTOSIZE );
    imshow( "Display Image" , image );
    waitKey( 0 );
    
    return 0;
}
