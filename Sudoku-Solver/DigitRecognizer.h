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
using cv::FileStorage;

#include "opencv2/ml/ml.hpp"


/**
 * A neural network trained by the MNIST dataset to
 * recognize digits.
 */
class DigitRecognizer {
    
friend int main( int argc , const char * argv[] );
    
private:
    
    struct TrainingDatum {
        int numRows;
        int numCols;
        unsigned char numericLabel;
        Mat image;
        Mat label;
        
        TrainingDatum( int imageRows , int imageCols , int numLabels ) :
            numRows( imageRows )
            , numCols( imageCols )
            , image( Mat( Size( numRows*numCols , 1 ) , CV_32F ) )
            , label( Mat( Size( numLabels , 1 ) , CV_32F ) )
        {

        }
        
        void display() {
            ImageDisplay disp;
            ostringstream title;
            Mat reshapedImage = Mat( numRows , numCols , CV_8UC1 );
            for ( int row = 0 ; row < numRows ; ++row ) {
                for ( int col = 0 ; col < numCols ; ++col ) {
                    reshapedImage.at<unsigned char>( row , col ) =
                        static_cast<unsigned char>( image.at<float>( row*numCols + col ) *255.0f );
                }
            }
            title << "This is a " << static_cast<int>(numericLabel);
            disp.showImage( title.str() , reshapedImage);
        }
    };
    
    vector< TrainingDatum > _trainingData;
    vector< TrainingDatum > _testData;
    
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
    void readImage( ifstream& ifs , int rows , int cols , TrainingDatum& buffer ) {
        unsigned char dataBuffer;
        for ( int r=0 ; r<rows ; ++r ) {
            for ( int c=0 ; c<cols ; ++c ) {
                readByte( ifs , dataBuffer );
                buffer.image.at<float>( r*cols+c ) = dataBuffer/255.0f;
            }
        }
    }
    
    void readLabel( ifstream& ifs , TrainingDatum& buffer ) {
        unsigned char dataBuffer;
        readByte( ifs , dataBuffer );
        buffer.label = Mat::zeros( 1 , 10 , CV_32F );
        buffer.label.at<float>( 0 , dataBuffer ) = 1.0f;
        buffer.numericLabel = dataBuffer;
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
        _trainingData.reserve( numImages );
        for ( int i=0 ; i<numImages ; ++i ) {
            _trainingData.push_back( TrainingDatum( numRows , numCols , 10 ) );
            readImage( ifs , numRows , numCols , _trainingData[ i ] );
        }
        
        //finished reading in training data - reuse stream to read in
        //training labels
        ifs.close();
        
        //read in the labels for each image
        ifs.open( trainLabelFile );
        readInt( ifs , magicNumber );
        
        readInt( ifs , numImages );
        
        for ( int i=0 ; i<numImages ; ++i ) {
            readLabel( ifs , _trainingData[ i ] );
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
        
        _testData.reserve( numImages );
        for ( int i=0 ; i<numImages ; ++i ) {
            _testData.push_back( TrainingDatum( numRows , numCols , 10 ) );
            readImage( ifs , numRows , numCols , _testData[ i ] );
        }
        
        ifs.close();
        
        ifs.open( testLabelFile );
        
        readInt( ifs , magicNumber );
        readInt( ifs , numImages );

        for ( int i=0 ; i<numImages ; ++i ) {
            readLabel( ifs , _testData[ i ] );
        }
    }
    
public:
    
    DigitRecognizer() {
        _neuralNetwork = cv::ml::ANN_MLP::create();
    }
    
    /**
     * Loads the weights of the digit recognition neural network so that
     * it doesn't have to be trained (a time-consuming effort)
     */
    void load( const string& filename ) {
        FileStorage fs( filename , FileStorage::READ );
        _neuralNetwork->read( fs.root() );
    }
    
    /**
     * Saves the weights of the neural network so that it doesn't
     * have to be trained again in future iterations.
     */
    void save( const string& filename ) {
        FileStorage fs( filename , FileStorage::WRITE );
        _neuralNetwork->write( fs );
    }
    
    void train( const string& trainImageFile , const string& trainLabelFile ) {
        readTrainingData( trainImageFile , trainLabelFile );
        
        Mat layerSizes = Mat( 3 , 1 , CV_32SC1 );
        int numFeatures = _trainingData[ 0 ].numRows * _trainingData[ 0 ].numCols;
        layerSizes.row( 0 ) = Scalar( numFeatures );
        layerSizes.row( 1 ) = Scalar( 25 );
        //layerSizes.row( 2 ) = Scalar( 4 );
        layerSizes.row( 2 ) = Scalar( 10 );
        _neuralNetwork->setLayerSizes( layerSizes );
        _neuralNetwork->setActivationFunction( cv::ml::ANN_MLP::SIGMOID_SYM );
        
        cout << "Training Data dimensions: " << _trainingData.size() <<
                        " images, " << _trainingData.size() << " labels" << endl;
        
        Mat trainingData = Mat( _trainingData.size() , numFeatures , CV_32F );
        for ( int i=0 ; i<_trainingData.size() ; ++i ) {
            _trainingData[ i ].image.copyTo( trainingData.row( i ) );
        }
        
        Mat trainingLabels = Mat( _trainingData.size() , 10 , CV_32F );
        for ( int i=0 ; i<_trainingData.size() ; ++i ) {
            _trainingData[ i ].label.copyTo( trainingLabels.row( i ) );
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
        int numFeatures = _testData[ 0 ].numRows * _testData[ 0 ].numCols;
        for ( int i=0 ; i<_testData.size() ; ++i ) {
            
            Mat testData = Mat( 1 , numFeatures , CV_32F );
            _testData[ i ].image.copyTo( testData.row( 0 ) );
            
            int prediction = predict( testData );
            int actual = static_cast<int>( _testData[ i ].numericLabel );
            cout << "Prediction for " << i << ": " << _neuralNetwork->predict( testData ) << endl;
            
            cout << "Actual for " << i << ": " << actual << endl;
            ++total;
            if ( actual == prediction ) {
                ++correct;
            }
        }
        
        cout << "Test set accuracy: " << (1.0*correct/total) << endl;
    }
    
    /**
     * Takes an image as a 28x28 grid of pixels and outputs the
     * neural network's prediction as to what number it is (0-9).
     */
    int predict( const Mat& img ) const {
        return static_cast<int>( _neuralNetwork->predict( img ) );
    }
    
    
};

#endif
