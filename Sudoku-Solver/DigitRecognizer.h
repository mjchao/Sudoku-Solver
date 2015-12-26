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
using cv::Scalar;

#include "opencv2/ml/ml.hpp"

/**
 * A neural network trained by the MNIST dataset to
 * recognize digits.
 */
class DigitRecognizer {
    
private:
    
    vector<vector<unsigned char>> _trainingData;
    vector<vector<unsigned char>> _trainingLabels;
    
    vector<vector<unsigned char>> _testData;
    vector<vector<unsigned char>> _testLabels;
    
    cv::Ptr<cv::ml::ANN_MLP> _neuralNetwork;
    
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
    
    void readLabel( ifstream& ifs , vector<unsigned char>& buffer ) {
        unsigned char dataBuffer;
        readByte( ifs , dataBuffer );
        buffer.resize( 10 , 0 );
        buffer[ static_cast<int>(dataBuffer) ] = 1;
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
        
        _trainingLabels.resize( numImages );
        for ( int i=0 ; i<numImages ; ++i ) {
            readLabel( ifs , _trainingLabels[ i ] );
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
        
        _testLabels.resize( numImages );
        for ( int i=0 ; i<numImages ; ++i ) {
            readLabel( ifs , _testLabels[ i ] );
        }
    }
    
public:
    
    DigitRecognizer() {
        
    }
    
    /*
    ~DigitRecognizer() {
        //_neuralNetwork.release();
    }
    
    DigitRecognizer( const DigitRecognizer& other ) {
        if ( !_neuralNetwork.empty() ) {
            _neuralNetwork.release();
        }
        _neuralNetwork = other._neuralNetwork;
    }
    
    DigitRecognizer& operator=( const DigitRecognizer& other ) {
        if ( !_neuralNetwork.empty() ) {
            _neuralNetwork.release();
        }
        _neuralNetwork = other._neuralNetwork;
        return *this;
    }*/
    
    void train( const string& trainImageFile , const string& trainLabelFile ) {
        readTrainingData( trainImageFile , trainLabelFile );
        
        _neuralNetwork = cv::ml::ANN_MLP::create();
        Mat layerSizes = Mat( 3 , 1 , CV_32SC1 );
        layerSizes.row( 0 ) = Scalar( _trainingData[ 0 ].size() );
        layerSizes.row( 1 ) = Scalar( 25 );
        layerSizes.row( 2 ) = Scalar( 10 );
        _neuralNetwork->setLayerSizes( layerSizes );
        _neuralNetwork->setActivationFunction( cv::ml::ANN_MLP::SIGMOID_SYM );
        cout << "Training Data dimensions: " << _trainingData.size() << " " << _trainingData[ 0 ].size() << endl;
        cout << "Training Label dimensions: " << _trainingLabels.size() << " " << _trainingLabels[ 0 ].size() << endl;
        
        Mat trainingData = Mat( _trainingData.size() , _trainingData[ 0 ].size() , CV_32F );
        for ( int i=0 ; i<_trainingData.size() ; ++i ) {
            for ( int j=0 ; j<_trainingData[ i ].size() ; ++j ) {
                trainingData.at<float>(i , j) = _trainingData[ i ][ j ]/255.0;
            }
        }
        
        Mat trainingLabels = Mat( _trainingLabels.size() , _trainingLabels[ 0 ].size() , CV_32F );
        for ( int i=0 ; i<_trainingLabels.size() ; ++i ) {
            for ( int j=0 ; j<_trainingLabels[ i ].size() ; ++j ) {
                trainingLabels.at<float>(i , j) = _trainingLabels[ i ][ j ] * 1.0f;
            }
        }
        
        if ( !_neuralNetwork->train( trainingData , cv::ml::ROW_SAMPLE , trainingLabels ) ) {
            cout << "Training failed" << endl;
            exit( 1 );
        }
    }
    
    void test( const string& testImageFile , const string& testLabelFile ) {
        readTestData( testImageFile , testLabelFile );
        
        int correct = 0;
        int total = 0;
        for ( int i=0 ; i<_testData.size() ; ++i ) {
            
            Mat testData = Mat( 1 , _testData[ 0 ].size() , CV_32F );
            for ( int j=0 ; j<_testData[ i ].size() ; ++j ) {
                testData.at<float>( 0 , j ) = _testData[ i ][ j ]/255.0;
            }
            
            float prediction = _neuralNetwork->predict( testData );
            cout << "Prediction for " << i << ": " << _neuralNetwork->predict( testData ) << endl;
            
            float actual = 0;
            for ( int j=0 ; j<_testLabels[ i ].size() ; ++j ) {
                if ( _testLabels[ i ][ j ] == 1 ) {
                    actual = j;
                    break;
                }
            }
            cout << "Actual for " << i << ": " << actual << endl;
            ++total;
            if ( actual == prediction ) {
                ++correct;
            }
        }
        
        cout << "Accuracy: " << (1.0*correct/total) << endl;
    }
    
    
};

#endif
