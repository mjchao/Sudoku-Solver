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
#include "PuzzleReader.h"

#include "opencv2/core/core.hpp"
using cv::Mat;

#define SUDOKU_PIZZLE_FILE "/Users/mjchao/Desktop/Sudoku-Solver/puzzle2.jpg"
#define DIGIT_TRAIN_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/train-images.idx3-ubyte"
#define DIGIT_TRAIN_LABEL_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/train-labels.idx1-ubyte"
#define DIGIT_TEST_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/t10k-images.idx3-ubyte"
#define DIGIT_TEST_LABEL_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/t10k-labels.idx1-ubyte"
#define DIGIT_NN_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/nndata.xml"


int main(int argc, const char * argv[]) {
    
    
    Mat puzzle;
    
    //Path    /Users/mjchao/Desktop/Sudoku-Solver/puzzle.jpg
    //cin >> imageFilename;
    puzzle = imread( SUDOKU_PIZZLE_FILE , 0 );

    PuzzleExtractor puzzleExtractor( puzzle );
    Mat isolatedPuzzle = puzzleExtractor.extractPuzzle();
    //*/
    
    DigitRecognizer digitRecognizer;
    
    /*
    digitRecognizer.train( DIGIT_TRAIN_DATA_FILE , DIGIT_TRAIN_LABEL_FILE );
    digitRecognizer.test( DIGIT_TEST_DATA_FILE , DIGIT_TEST_LABEL_FILE );
    digitRecognizer.save( DIGIT_NN_DATA_FILE );
    //*/
    
    digitRecognizer.load( DIGIT_NN_DATA_FILE );
    //digitRecognizer.test( DIGIT_TEST_DATA_FILE , DIGIT_TEST_LABEL_FILE );
    //*/
    
    PuzzleReader reader( isolatedPuzzle , digitRecognizer );
    vector<vector<int>> digits;
    reader.getDigits( digits );
    
    //cout << digitRecognizer._testData[ 0 ].image << endl;
    
    return 0;
}
