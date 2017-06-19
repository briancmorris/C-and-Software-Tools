/**
    @file puzzle.c
    @author Brian Morris (bcmorri3)

    The puzzle program accepts an input file from command-line arguments and
    displays the contents of the file as a crossword puzzle based on the format
    specifiers found within the input file. If there are any errors with command-line arguments,
    the file cannot be opened, or the file is formatted incorrectly, the program is exited
    with a status of 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

/** The exit status of the program if an error is encountered. */
#define ERROR_STATUS 1

/** The maximum number of rows a grid may have. */
#define MAX_ROWS 40

/** The maximum number of columns a grid may have. */
#define MAX_COLS 40

/** The maximum number of letters that a word can have in the puzzle. */
#define MAX_WORD_LEN 10

/** The number of valid command-line arguments that must be input to run the program. */
#define VALID_ARGS 2

/** The position of the file name found within the command-line arguments array. */
#define FILE_NAME_ARGS 1

/** The length of the array used to store a word from input to ensure there is no overflow. */
#define WORD_SIZE 50

/** The number of arguments that fscanf must find in order to have a valid grid format. */
#define FSCANF_GRID 3

/** The number of arguments that fscanf must find in order to have a valid word. */
#define FSCANF_WORD 4

/**
    The main function is the starting point of the program, reads words from the input file,
    and uses the functions described by the grid.h header file to display the crossword puzzle.
    The main function is responsible for all error checking within the program. Predominantly,
    this is related to input file formatting. Errors include: an invalid number of command-line
    arguments, being unable to open the input file, incorrect puzzle format specifiers, words that
    have not been formatted correctly in the input file, words that are too long, and overlapping
    words that do not share the same intersecting character. If an error has been encountered,
    main returns the error status of 1.

    @param argc the number of command-line arguments.
    @param argv array of command-line arguments.

    @return program exit status
 */
int main( int argc, char *argv[] )
{
    // Handle invalid number of command-line arguments.
    if ( argc != VALID_ARGS ) {
        fprintf( stderr, "usage: puzzle <input-file>\n" );
        return ERROR_STATUS;
    }

    // Input file.
    FILE *input = fopen( argv[ FILE_NAME_ARGS], "r" );

    // If the file can't be opened.
    if ( !input ) {
        fprintf( stderr, "usage: puzzle <input-file>\n" );
        return ERROR_STATUS;
    }

    // The number of rows specified by the input file.
    int rows;
    // The number of columns specified by the input file.
    int cols;
    // The number of words in the puzzle specified by the input file.
    int numWords;

    // Make sure the correct number of grid specifiers are provided.
    if ( fscanf( input, "%d %d\n%d\n", &rows, &cols, &numWords ) != FSCANF_GRID ) {
        fprintf( stderr, "Invalid input file\n" );
        return ERROR_STATUS;
    }

    // Make sure that rows and cols don't exceed the maximum limit.
    if ( rows > MAX_ROWS || cols > MAX_COLS ) {
        fprintf( stderr, "Invalid input file\n" );
        return ERROR_STATUS;
    }

    // Create grid for puzzle and fill with spaces to remove garbage.
    char grid[ rows ][ cols ];
    for( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            grid[ i ][ j ] = ' ';
        }
    }

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

    // Loop until there is no more input or the number of required words have been found.
    while( fscanf( input, "%c %d %d %s\n", &orientation, &rpos, &cpos, word ) == FSCANF_WORD
                   && wordCount < numWords ) {

        // The length of the word.
        int len = strlen( word );

        // If the length exceeds the maximum limit.
        if ( len > MAX_WORD_LEN ) {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }

        // If the orientation is not a valid character.
        if ( orientation != 'H' && orientation != 'V') {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }

        // If the length would exceed array bounds horizontally.
        if ( orientation == 'H' && ( cpos + len ) > cols ) {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }

        // If the length would exceed array bounds vertically.
        if ( orientation == 'V' && ( rpos + len ) > rows ) {
            fprintf( stderr, "Invalid input file\n" );
            return ERROR_STATUS;
        }

        // Ensure all letters within the word are valid.
        for ( int i = 0; i < len; i++ ) {
            if( word[ i ] < 'A' || word[ i ] > 'Z' ) {
                fprintf( stderr, "Invalid input file\n" );
                return ERROR_STATUS;
            }
        }

        // Handle a word with horizontal orientation.
        if ( orientation == 'H' ) {
            // The index position in the word array.
            int wordPos = 0;

            // Ensure that any intersecting characters are the same character.
            for (int i = cpos; i < cpos + len; i++ ) {
                if ( grid[ rpos ][ i ] != ' ' && grid[ rpos ][ i ] != word[ wordPos ] ) {
                    fprintf( stderr, "Invalid input file\n" );
                    return ERROR_STATUS;
                }
                wordPos++;
            }

            // Add the word to the grid array if it survived error checking.
            writeHorizontal( rpos, cpos, word, rows, cols, grid );
        } else {
            // The index position in the word array.
            int wordPos = 0;

            // Ensure that any intersecting characters are the same character.
            for (int i = rpos; i < rpos + len; i++ ) {
                if ( grid[ i ][ cpos ] != ' ' && grid[ i ][ cpos ] != word[ wordPos ] ) {
                    fprintf( stderr, "Invalid input file\n" );
                    return ERROR_STATUS;
                }
            wordPos++;
            }

            // Add the word to the grid array if it survived error checking.
            writeVertical( rpos, cpos, word, rows, cols, grid );
        }

        // Increase wordCount for while loop.
        wordCount++;
    }

    // If the input file did not contain the number of specified words.
    if ( wordCount < numWords ) {
        fprintf( stderr, "Invalid input file\n" );
        return ERROR_STATUS;
    } else {
        // Print the puzzle.
        printGrid( rows, cols, grid );
    }

    return EXIT_SUCCESS;
}
