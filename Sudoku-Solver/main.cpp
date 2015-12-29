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

//DIGIT dataset refers to MNIST dataset which seems too clean and doesn't
//generalize well to printed characters
#define DIGIT_TRAIN_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/train-images.idx3-ubyte"
#define DIGIT_TRAIN_LABEL_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/train-labels.idx1-ubyte"
#define DIGIT_TEST_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/t10k-images.idx3-ubyte"
#define DIGIT_TEST_LABEL_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/t10k-labels.idx1-ubyte"
#define DIGIT_NN_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/digits/nndata.xml"

//DIGIT2 dataset refers to custom generated set that is more dirty and
//of printed characters. Hopefully DIGIT2 will generalize better to printed
//characters
#define DIGIT2_TRAIN_DIRECTORY "/Users/mjchao/Desktop/Sudoku-Solver/PhotoOCRTraining/train"
#define DIGIT2_TEST_DIRECTORY "/Users/mjchao/Desktop/Sudoku-Solver/PhotoOCRTraining/test"
#define DIGIT2_NN_DATA_FILE "/Users/mjchao/Desktop/Sudoku-Solver/PhotoOCRTraining/nndata.xml"

int main(int argc, const char * argv[]) {
    
    Mat puzzle;
    
    //Path    /Users/mjchao/Desktop/Sudoku-Solver/puzzle.jpg
    //cin >> imageFilename;
    puzzle = imread( SUDOKU_PIZZLE_FILE , 0 );

    PuzzleExtractor puzzleExtractor( puzzle );
    Mat isolatedPuzzle = puzzleExtractor.extractPuzzle();
    //*/
    
    DigitRecognizer digitRecognizer;
    
    
    digitRecognizer.train( DIGIT2_TRAIN_DIRECTORY );
    digitRecognizer.test( DIGIT2_TEST_DIRECTORY );
    digitRecognizer.save( DIGIT2_NN_DATA_FILE );
    //*/
    
    //digitRecognizer.load( DIGIT_NN_DATA_FILE );
    //digitRecognizer.test( DIGIT2_TEST_DIRECTORY );
    //*/
    
    /*
    PuzzleReader reader( isolatedPuzzle , digitRecognizer );
    vector<vector<int>> digits;
    reader.getDigits( digits );
    //*/
    
    //cout << digitRecognizer._testData[ 0 ].image << endl;
    
    return 0;
}


