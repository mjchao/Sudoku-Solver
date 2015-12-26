//
//  DigitRecognizer.h
//  Sudoku-Solver
//
//  Created by Mickey on 12/25/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#ifndef Sudoku_Solver_DigitRecognizer_h
#define Sudoku_Solver_DigitRecognizer_h

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include "Utils.h"

#include "opencv2/core/core.hpp"
using cv::Mat;

/**
 * A neural network trained by the MNIST dataset to
 * recognize digits.
 */
class DigitRecognizer {
    
private:
    
    vector<vector<unsigned char>> _trainingData;
    vector<unsigned char> _trainingLabels;
    
    vector<vector<unsigned char>> _testData;
    vector<unsigned char> _testLabels;
    
    void readInt( ifstream& ifs , int& rtn ) {
        rtn = 0;
        
        //reverse the bytes because
        //the processor (intel) is little-endian
        //but the data is stored in big-endian format
        rtn += (ifs.get() << 24);
        rtn += (ifs.get() << 16);
        rtn += (ifs.get() << 8);
        rtn += (ifs.get() << 0);
    }
    
    void readByte( ifstream& ifs , unsigned char& rtn ) {
        rtn = static_cast<unsigned char>( ifs.get() );
    }
    
    /**
     * Reads an image with the given dimensions into the
     * given buffer.
     */
    void readImage( ifstream& ifs , int rows , int cols , vector<unsigned char>& buffer ) {
        unsigned char dataBuffer;
        for ( int r=0 ; r<rows ; ++r ) {
            for ( int c=0 ; c<cols ; ++c ) {
                readByte( ifs , dataBuffer );
                buffer[ r*cols + c ] = dataBuffer;
            }
        }
    }
    
    /**
     * Reads in the training data for the digit classifier
     */
    void readTrainingData( const string& trainImageFile , const string& trainLabelFile ) {
        ifstream ifs;
        ifs.open( trainImageFile.c_str() , std::ios::binary );
        
        int magicNumber;
        readInt( ifs , magicNumber );
        
        int numImages;
        readInt( ifs , numImages );
        
        int numRows;
        readInt( ifs , numRows );
        
        int numCols;
        readInt( ifs , numCols );
        
        //read in image data byte by byte
        //images are 28 x 28 and each byte from 0-255
        //represents the color of the pixel
        //where 0 is completely white background and
        //255 is completely black foreground
        _trainingData.resize( numImages , vector<unsigned char>( numRows*numCols ) );
        for ( int i=0 ; i<numImages ; ++i ) {
            readImage( ifs , numRows , numCols , _trainingData[ i ] );
        }
        
        //finished reading in training data - reuse stream to read in
        //training labels
        ifs.close();
        
        //read in the labels for each image
        ifs.open( trainLabelFile );
        readInt( ifs , magicNumber );
        
        readInt( ifs , numImages );
        
        unsigned char labelBuffer;
        _trainingLabels.resize( numImages );
        for ( int i=0 ; i<numImages ; ++i ) {
            readByte( ifs , labelBuffer );
            _trainingLabels[ i ] = labelBuffer;
        }
    }
    
    /**
     * Reads in the test data for the digit classifier
     */
    void readTestData( const string& testImageFile , const string& testLabelFile ) {
        ifstream ifs;
        ifs.open( testImageFile );
        
        int magicNumber;
        readInt( ifs , magicNumber );
        
        int numImages;
        readInt( ifs , numImages );
        
        int numRows;
        readInt( ifs , numRows );
        
        int numCols;
        readInt( ifs , numCols );
        
        _testData.resize( numImages , vector<unsigned char>( numRows*numCols ) );
        for ( int i=0 ; i<numImages ; ++i ) {
            readImage( ifs , numRows , numCols , _testData[ i ] );
        }
        
        ifs.close();
        
        ifs.open( testLabelFile );
        
        readInt( ifs , magicNumber );
        readInt( ifs , numImages );
        
        unsigned char labelBuffer;
        _testLabels.resize( numImages );
        for ( int i=0 ; i<numImages ; ++i ) {
            readByte( ifs , labelBuffer );
            _testLabels[ i ] = labelBuffer;
        }
    }
    
public:
    
    DigitRecognizer() {
        
    }
    
    void train( const string& trainImageFile , const string& trainLabelFile ) {
        readTrainingData( trainImageFile , trainLabelFile );
    }
    
    void test( const string& testImageFile , const string& testLabelFile ) {
        readTestData( testImageFile , testLabelFile );
        
        Mat testImg( 28 , 28 , CV_8UC1 );
        ImageDisplay disp;
        for ( int i=0 ; i<25 ; ++i ) {
            for ( int r=0 ; r<28 ; ++r ) {
                for ( int c=0 ; c<28 ; ++c ) {
                    testImg.at<unsigned char>( r , c ) = _testData[ i ][ r*28+c ];
                }
            }
            ostringstream title;
            title << "This is a " << static_cast<int>(_testLabels[ i ]);
            disp.showImage( title.str() , testImg );
        }
    }
    
    
};

#endif
