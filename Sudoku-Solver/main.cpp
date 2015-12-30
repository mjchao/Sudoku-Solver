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
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include "PuzzleExtractor.h"
#include "DigitRecognizer.h"
#include "PuzzleReader.h"
#include "Solver.h"
#include "PuzzleCompleter.h"

#include "opencv2/core/core.hpp"
using cv::Mat;
using cv::imshow;

//REMINDER: Puzzle3 is PNG not JPG
#define SUDOKU_PUZZLE_FILE "/Users/mjchao/Desktop/Sudoku-Solver/puzzle3.png"
#define SUDOKU_SOLUTION_FILE "/Users/mjchao/Desktop/Sudoku-Solver/solution3.png"

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
    puzzle = imread( SUDOKU_PUZZLE_FILE , 0 );
    
    if ( !puzzle.data ) {
        cerr << "Invalid puzzle file." << endl;
        exit( 1 );
    }
    
    imshow( "Original Image" , puzzle );
    waitKey( 0 );
    

    PuzzleExtractor puzzleExtractor( puzzle );
    Mat isolatedPuzzle = puzzleExtractor.extractPuzzle();
    
    imshow( "Extracted Puzzle" , isolatedPuzzle );
    waitKey( 0 );
    
    //*/
    
    DigitRecognizer digitRecognizer;
    
    /*
    digitRecognizer.train( DIGIT2_TRAIN_DIRECTORY );
    digitRecognizer.test( DIGIT2_TEST_DIRECTORY );
    digitRecognizer.save( DIGIT2_NN_DATA_FILE );
    //*/
    
    digitRecognizer.load( DIGIT2_NN_DATA_FILE );
    //digitRecognizer.test( DIGIT2_TEST_DIRECTORY );
    //*/
    
    PuzzleReader reader( isolatedPuzzle , digitRecognizer );
    vector<vector<int>> digits;
    reader.getDigits( digits , true );
    //*/
    
    vector<vector<int>> solvedPuzzle = digits;
    
    cout << "Original puzzle: " << endl;
    for ( int i=0 ; i<9 ; ++i ) {
        for ( int j=0 ; j<9 ; ++j ) {
            if ( solvedPuzzle[ i ][ j ] != -1 ) {
                cout << solvedPuzzle[ i ][ j ] << " ";
            }
            else {
                cout << "?" << " ";
            }
        }
        cout << endl;
    }
    
    cout << endl;
    Solver solver;
    cout << "Solver status: " << solver.solve( solvedPuzzle ) << endl;
    cout << endl;
    
    cout << "Solved puzzle: " << endl;
    for ( int i=0 ; i<9 ; ++i ) {
        for ( int j=0 ; j<9 ; ++j ) {
            if ( solvedPuzzle[ i ][ j ] != -1 ) {
                cout << solvedPuzzle[ i ][ j ] << " ";
            }
            else {
                cout << "?" << " ";
            }
        }
        cout << endl;
    }
    
    PuzzleCompleter completer;
    Mat solution = isolatedPuzzle;
    cvtColor( solution , solution , CV_GRAY2RGB );
    completer.complete( solution , digits , solvedPuzzle );
    
    imshow( "Solution" , solution );
    waitKey( 0 );
    imwrite( SUDOKU_SOLUTION_FILE , solution );
    return 0;
}


