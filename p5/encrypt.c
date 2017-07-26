#include <stdio.h>
#include <stdlib.h>
#include "codes.h"
#include "bits.h"

/** The number of arguments that are required to encrypt a file. */
#define REQUIRED_ARGS 3

/** The exit status of the program if the given input or output files are invalid. */
#define INVALID 1

int main( int argc, char *argv[] )
{
    // If the number of required arguments is invalid, print error message.
    if ( argc != REQUIRED_ARGS ) {
        fprintf( stderr, "usage: encrypt <infile> <outfile>\n" );
        return INVALID;
    }

    // Input file pointer.
    FILE *input = fopen( argv[ 1 ], "r" );

    // If the input file pointer is NULL, print error message.
    if ( !input ) {
        fprintf( stderr, "%s: No such file or directory\n", argv[ 1 ] );
        return INVALID;
    }

    // Output file pointer.
    FILE *output = fopen( argv[ 2 ], "wb" );

    // If the output file pointer is NULL, print error message and close input.
    if ( !output ) {
        fclose( input );
        fprintf( stderr, "%s: No such file or directory\n", argv[ 2 ] );
        return INVALID;
    }

    // Initialize buffer to empty values.
    BitBuffer buffer = { .bits = 0x00, .bcount = 0 };

    // Loop until EOF.
    int ch;
    while ( ( ch = fgetc( input ) ) != EOF ) {
        // Get the binary code that represents the character.
        int code = symToCode( ch );
        // Get the number of bits used for the binary code.
        int nbits = bitsInCode( ch );

        // If the code is invalid, print error messsage and close files.
        if ( code == INVALID_CODE || nbits == INVALID_CODE ) {
            fclose( input );
            fclose( output );
            fprintf( stderr, "Invalid file\n" );
            return INVALID;
        }

        // Write the bits to the output file if valid.
        writeBits( code, nbits, &buffer, output );
    }

    // If the buffer still contains binary code, flush it.
    if( buffer.bcount != 0 ) {
        flushBits( &buffer, output );
    }

    // Close files.
    fclose( input );
    fclose( output );

    // Program success.
    return EXIT_SUCCESS;
}
