/**
    @file decrypt.c
    @author Brian Morris (bcmorri3)

    The decrypt program accepts command line arguments to decrypt a binary
    file and convert its contents to a plain text file. This program takes
    advantage of the codes.h and bits.h header files to properly decrypt
    the contents of the binary file.
*/

#include <stdio.h>
#include <stdlib.h>
#include "codes.h"
#include "bits.h"

/** The number of arguments that are required to decrypt a file. */
#define REQUIRED_ARGS 3

/** The exit status of the program if the given input or output files are invalid. */
#define INVALID 1

/**
    The main function is the starting point of the program. Responsible for controlling
    file IO and invalid use cases. If the use case was deemed invalid, 1 is returned. Otherwise
    a standard exit status of 0 is returned.

    @param argc The number of command line arguments.
    @param argv The array containing the command line arguments.

    @return Exit status for the program. 0 if valid, 1 if invalid.
 */
int main( int argc, char *argv[] )
{
    // If the required number of arguments are not given, print error message.
    if ( argc != REQUIRED_ARGS ) {
        fprintf( stderr, "usage: decrypt <infile> <outfile>\n" );
        return INVALID;
    }

    // Input file pointer.
    FILE *input = fopen( argv[ 1 ], "rb" );

    // If the input file pointer is NULL, print error message.
    if ( !input ) {
        fprintf( stderr, "%s: No such file or directory\n", argv[ 1 ] );
        return INVALID;
    }

    // Output file pointer.
    FILE *output = fopen( argv[ 2 ], "w" );

    // If the output file pointer is NULL,close input and print error message.
    if ( !output ) {
        fclose( input );
        fprintf( stderr, "%s: No such file or directory\n", argv[ 2 ] );
        return INVALID;
    }

    // Initialize an empty buffer.
    BitBuffer buffer = { .bits = 0x00, .bcount = 0 };

    // Used to check for EOF.
    int ch;
    // While ch is not EOF.
    while ( ( ch = fgetc( input ) ) != EOF ) {
        // Put the character back into the file.
        ungetc( ch, input );

        // Encrypted code read from input file.
        int code = readBits( &buffer, input );
        // ASCII Symbol to write to output.
        int sym;

        // If the code is invalid, close files and return 1.
        if ( code < 0 ) {
            fclose( input );
            fclose( output );
            fprintf( stderr, "Invalid file\n" );
            return INVALID;
        } else {
            sym = codeToSym( code );
        }

        // If the symbol is invalid, close files and return 1.
        if ( sym < 0 ) {
            fclose( input );
            fclose( output );
            fprintf( stderr, "Invalid file\n" );
            return INVALID;
        } else {
            fputc( sym, output );
        }
    }

    // Close the files.
    fclose( input );
    fclose( output );

    // Program successful.
    return EXIT_SUCCESS;
}
