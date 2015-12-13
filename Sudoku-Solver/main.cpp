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

#include <vector>
using std::vector;

#include <string>
using std::string;

#include "GridExtractor.h"

#include "opencv2/opencv.hpp"
using namespace cv;


int main(int argc, const char * argv[]) {
    
    Mat puzzle;
    
    //Path    /Users/mjchao/Desktop/Sudoku-Solver/puzzle.jpg
    string imageFilename = "/Users/mjchao/Desktop/Sudoku-Solver/puzzle.jpg";
    //cin >> imageFilename;
    puzzle = imread( imageFilename.c_str() , 0 );
    
    GridExtractor gridExtractor( puzzle );
    
    vector< Vec2f > gridLines;
    gridExtractor.extractGrid( gridLines );
    
    return 0;
}
