#include <stdio.h>
#include <string.h>
#include "grid.h"

void writeHorizontal( int rpos, int cpos, char word[], int rows, int cols, char grid[ rows ][ cols ] )
{
    int gridPos = cpos;
    int wordPos = 0;
    while ( word ) {
        grid[ rpos ][ gridPos ] = word[ wordPos ];
        gridPos++;
        wordPos++;
    }
}

void writeVertical( int rpos, int cpos, char word[], int rows, int cols, char grid [ rows ][ cols ] )
{
    int gridPos = rpos;
    int wordPos = 0;
    while ( word ) {
        grid[ gridPos ][ cpos ] = word[ wordPos ];
        gridPos++;
        wordPos++;
    }
}

void printGrid ( int rows, int cols, char board[ rows ][ cols ] )
{
    char ch;
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
             ch = board[ i ][ j ];
             if ( ch >= 'A' && ch <= 'Z') {
                 printf( "%c", ch );
             } else {
                 printf( " " );
             }
        }
        printf( "\n" );
    }
}