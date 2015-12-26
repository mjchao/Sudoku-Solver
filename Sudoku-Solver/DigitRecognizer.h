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

/**
 * A neural network trained by the MNIST dataset to
 * recognize digits.
 */
class DigitRecognizer {
    
private:
    
    vector<vector<unsigned char>> _trainingData;
    vector<unsigned char> _trainingLabels;
    
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
    
public:
    
    DigitRecognizer() {
        
    }
    
    void train( const string& trainImageFile , const string& trainLabelFile ) {
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
        
        numImages = 1;
        
        //read in image data byte by byte
        //images are 28 x 28 and each byte from 0-255
        //represents the color of the pixel
        //where 0 is completely white background and
        //255 is completely black foreground
        unsigned char inputBuffer;
        _trainingData.resize( numImages , vector<unsigned char>( numRows*numCols ) );
        for ( int i=0 ; i<numImages ; ++i ) {
            for ( int r=0 ; r<numRows ; ++r ) {
                for ( int c=0 ; c<numCols ; ++c ) {
                    readByte( ifs , inputBuffer );
                    _trainingData[ i ][ r*numCols+c ] = inputBuffer;
                }
            }
        }
        
        //finished reading in training data - reuse stream to read in
        //training labels
        ifs.close();
        
        //read in the labels for each image
        ifs.open( trainLabelFile );
        readInt( ifs , magicNumber );
        cout << magicNumber << endl;
        
        readInt( ifs , numImages );
        cout << numImages << endl;
        numImages = 25;
        
        unsigned char labelBuffer;
        _trainingLabels.resize( numImages );
        for ( int i=0 ; i<numImages ; ++i ) {
            readByte( ifs , labelBuffer );
            _trainingLabels[ i ] = labelBuffer;
            cout << static_cast<int>( labelBuffer ) << endl;
        }
    }
    
    
};

#endif
