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

#include "PuzzleExtractor.h"

#include "DigitRecognizer.h"

#include "opencv2/core/core.hpp"
using cv::Mat;

#define DIGIT_TRAIN_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/train-images.idx3-ubyte"
#define DIGIT_TRAIN_LABEL_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/train-labels.idx1-ubyte"
#define DIGIT_TEST_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/t10k-images.idx3-ubyte"
#define DIGIT_TEST_LABEL_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/t10k-labels.idx1-ubyte"
#define DIGIT_NN_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/nndata.xml"


int main(int argc, const char * argv[]) {
    
    /*
    Mat puzzle;
    
    //Path    /Users/mjchao/Desktop/Sudoku-Solver/puzzle.jpg
    string imageFilename = "/Users/mjchao/Desktop/Sudoku-Solver/puzzle.jpg";
    //cin >> imageFilename;
    puzzle = imread( imageFilename.c_str() , 0 );

    PuzzleExtractor puzzleExtractor( puzzle );
    puzzleExtractor.extractPuzzle();
    */
    
    DigitRecognizer d;
    /*
    d.train( DIGIT_TRAIN_DATA_FILE , DIGIT_TRAIN_LABEL_FILE );
    d.test( DIGIT_TEST_DATA_FILE , DIGIT_TEST_LABEL_FILE );
    d.save( DIGIT_NN_DATA_FILE );
    //*/
    
    d.load( DIGIT_NN_DATA_FILE );
    d.test( DIGIT_TEST_DATA_FILE , DIGIT_TEST_LABEL_FILE );
    //*/
    
    return 0;
}
