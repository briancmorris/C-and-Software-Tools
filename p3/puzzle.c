
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

/** The exit status of the program if an error is encountered. */
#define ERROR_STATUS 1

/** The maximum number of letters that a word can have in the puzzle. */
#define MAX_WORD_LEN 10

/** The number of valid command line arguments that must be input to run the program. */
#define VALID_ARGS 2

/** The position of the file name found within the command line arguments array. */
#define FILE_NAME_ARGS 1

/** The length of the array used to store a word from input to ensure there is no overflow. */
#define WORD_SIZE 50

/** The number of arguments that fscanf must find in order to have a valid grid. */
#define FSCANF_GRID 3

/** The number of arguments that fscanf must find in order to have a valid word. */
#define FSCANF_WORD 4

int main( int argc, char *argv[] )
{
    // Handle invalid number of command line arguments.
    if ( argc != VALID_ARGS ) {
        fprintf( stderr, "usage: puzzle <input-file>\n" );
        return ERROR_STATUS;
    }
    printf("Survived valid command line arguments.\n");

    // Input file.
    FILE *input = fopen( argv[ FILE_NAME_ARGS], "r" );

    // If the file can't be opened.
    if ( !input ) {
        fprintf( stderr, "usage: puzzle <input-file>\n" );
        return ERROR_STATUS;
    }
    printf("Survived opening file check.\n");
    
    // The number of rows specified by the input file.
    int rows;
    // The number of columns specified by the input file.
    int cols;
    // The number of words in the puzzle specified by the input file.
    int numWords;

    // Make sure the correct number of grid specifiers are input.
    if ( fscanf( input, "%d %d\n%d\n", &rows, &cols, &numWords ) != FSCANF_GRID ) {
        fprintf( stderr, "Invalid input file\n" );
        return ERROR_STATUS;
    }
    printf("Survived proper grid format.\n");

    // Make sure that rows and cols don't exceed the maximum limit.
    if ( rows > MAX_ROWS || cols > MAX_COLS ) {
        fprintf( stderr, "Invalid input file\n" );
        return ERROR_STATUS;
    }
    printf("Survived row/column check.\n");


    // Create grid for puzzle and array for word.
    char grid[ rows ][ cols ];

    // Number of words in the puzzle.
    int wordCount = 0;

    // Array to hold word from input.
    char word[ WORD_SIZE ];
    // Orientation of word in puzzle.
    char orientation;
    // Starting row position.
    int rpos;
    // Starting column position.
    int cpos;

    // Loop until there is no more input or the number of words in the puzzle have been reached.
    while( fscanf( input, "%c %d %d %s\n", &orientation, &rpos, &cpos, word ) == FSCANF_WORD
                   && wordCount < numWords ) {

        printf("Word is: %s\n", word);
        // The length of the word.
        int len = strlen( word );
        printf("Length is: %d\n", len );

        // If the length exceeds the maximum limit.
        if ( len > MAX_WORD_LEN ) {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }
        printf("Survived max length check.\n");
        // If the orientation is not a valid character.
        if ( orientation != 'H' && orientation != 'V') {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }
        printf("Survived proper orientation chars.\n");
        // If the length would exceed array bounds horizontally.
        if ( orientation == 'H' && cpos + len > cols ) {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }
        printf("Survived horizontal length check.\n");
        // If the length would exceed array bounds vertically.
        if ( orientation == 'V' && rpos + len > rows ) {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }
        printf("Survived vertical length check.\n");
        // Ensure all letters within the word are valid.
        for ( int i = 0; i < len; i++ ) {
            printf("i is: %d\n", i );
            if( word[ i ] < 'A' || word[ i ] > 'Z' ) {
                fprintf( stderr, "Invalid input file\n" );
                return ERROR_STATUS;
            }
            printf("Survived check: %d\n", i);
        }
        printf("Survived valid letters check.");

        // If the word survived error checking, put it in the puzzle based on its orientation.
        if ( orientation == 'H' ) {
            writeHorizontal( rpos, cpos, word, rows, cols, grid );
        } else {
            writeVertical( rpos, cpos, word, rows, cols, grid );
        }

        wordCount++;
    }

    // If the input file did not contain the number of specified words.
    if ( wordCount < numWords ) {
        fprintf( stderr, "Invalid input file\n" );
        return ERROR_STATUS;
    } else {
        printf("Survived proper word count check.\n");
        printGrid( rows, cols, grid );
        
    }

    //I did it!
    return EXIT_SUCCESS;
}
