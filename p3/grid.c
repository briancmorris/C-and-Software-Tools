/**
    @file grid.c
    @author Brian Morris (bcmorri3)

    This program implements the functions and behavior described in the grid.h
    header file. These functions allow for words to be stored in a two-dimensional
    char array horizontally or vertically. If desired, the array can be printed with
    a space between each character.
 */

#include <stdio.h>
#include <string.h>
#include "grid.h"

void writeHorizontal( int rpos, int cpos, char word[], int rows, int cols, char grid[rows][cols] )
{
    // The length of the word.
    int len = strlen( word );
    // The index position of the word.
    int wordPos = 0;

    for (int i = cpos; i < ( cpos + len ); i++ ) {
        grid[ rpos ][ i ] = word[ wordPos ];
        wordPos++;
    }
}

void writeVertical( int rpos, int cpos, char word[], int rows, int cols, char grid[rows][cols] )
{
    // The length of the word.
    int len = strlen( word );
    // The index position of the word.
    int wordPos = 0;

    for (int i = rpos; i < ( rpos + len ); i++ ) {
        grid[ i ][ cpos ] = word[ wordPos ];
        wordPos++;
    }
}

void printGrid ( int rows, int cols, char board[ rows ][ cols ] )
{
    // The character found at the position [ i ][ j ] of the board array.
    char ch;
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
             ch = board[ i ][ j ];
             // If it's not the final character of a row, print the char with a trailing space.
             if ( j < cols - 1) {
                 printf( "%c ", board[ i ][ j ] );
             } else {
                 printf( "%c", board[ i ][ j ] );
             }
        }
        // Start a new row.
        printf( "\n" );
    }
}
