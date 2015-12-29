//
//  Solver.h
//  Sudoku-Solver
//
//  Created by Mickey on 12/29/15.
//  Copyright (c) 2015 Mickey. All rights reserved.
//

#ifndef Sudoku_Solver_Solver_h
#define Sudoku_Solver_Solver_h

class Solver {
    
private:
    
    void printPuzzle( const vector<vector<int>>& puzzle ) {
        for ( int i=0 ; i<9 ; ++i ) {
            for ( int j=0 ; j<9 ; ++j ) {
                cout << puzzle[ i ][ j ] << " ";
            }
            cout << endl;
        }
    }
    
    bool canFillIn( int val , int row , int col , const vector<vector<int>>& partialSolution ) {
        //check row
        for ( int c=0 ; c<9 ; ++c ) {
            if ( partialSolution[ row ][ c ] == val ) {
                return false;
            }
        }
        
        //check col
        for ( int r=0 ; r<9 ; ++r ) {
            if ( partialSolution[ r ][ col ] == val ) {
                return false;
            }
        }
        
        //check 3x3 square
        int startRow = row/3 * 3;
        int endRow = startRow+3;
        int startCol = col/3 * 3;
        int endCol = startCol+3;
        for ( int i=startRow ; i<endRow ; ++i ) {
            for ( int j=startCol ; j<endCol ; ++j ) {
                if ( partialSolution[ i ][ j ] == val ) {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool search( vector<vector<int>>& puzzle , int row , int col ) {
        if ( row == -1 && col == -1 ) {
            
            //found solution
            return true;
        }
        
        int nextRow = -1;
        int nextCol = -1;
        
        bool done = false;
        for ( int r=row ; r<9 ; ++r ) {
            for ( int c=0 ; c<9 ; ++c ) {
                if ( !(r == row && c==col) && puzzle[ r ][ c ] == -1 ) {
                    nextRow = r;
                    nextCol = c;
                    done = true;
                    break;
                }
            }
            if ( done ) {
                break;
            }
        }
        
        for ( int i=1 ; i<=9 ; ++i ) {
            if ( canFillIn( i , row , col , puzzle ) ) {
                puzzle[ row ][ col ] = i;
                if ( search( puzzle , nextRow , nextCol ) == true ) {
                    return true;
                }
                puzzle[ row ][ col ] = -1;
            }
        }
        /*if ( row == 0 && col == 0 ) {
            printPuzzle( puzzle );
            cout << canFillIn( 1 , 0 , 0 , puzzle ) << endl;
        }*/
        return false;
    }
    
public:
    
    Solver() {
        
    }
    
    /**
     * Solve the given sudoku puzzle. -1 indicates an unknown square. Otherwise,
     * the digits should be from 1-9. Applies a backtracking algorithm.
     */
    bool solve( vector<vector<int>>& puzzle ) {
        int nextRow = -1;
        int nextCol = -1;
        
        bool done = false;
        for ( int r=0 ; r<9 ; ++r ) {
            for ( int c=0 ; c<9 ; ++c ) {
                if ( puzzle[ r ][ c ] == -1 ) {
                    nextRow = r;
                    nextCol = c;
                    done = true;
                    break;
                }
            }
            if ( done ) {
                break;
            }
        }
        
        return search( puzzle , nextRow , nextCol );
    }
};

#endif
