#include <stdio.h>
#include <stdlib.h>
#include "codes.h"
#include "bits.h"

/** The number of arguments that are required to decrypt a file. */
#define REQUIRED_ARGS 3

/** The exit status of the program if the given input or output files are invalid. */
#define INVALID 1

int main( int argc, char *argv[] )
{
    if ( argc != REQUIRED_ARGS ) {
        fprintf( stderr, "usage: decrypt <infile> <outfile>\n" );
        return INVALID;
    }

    FILE *input = fopen( argv[ 1 ], "rb" );

    if ( !input ) {
        fprintf( stderr, "%s: No such file or directory\n", argv[ 1 ] );
        return INVALID;
    }

    FILE *output = fopen( argv[ 2 ], "w" );

    if ( !output ) {
        fclose( input );
        fprintf( stderr, "%s: No such file or directory\n", argv[ 2 ] );
        return INVALID;
    }

    BitBuffer buffer = { .bits = 0x00, .bcount = 0 };
    int ch;
    while ( ( ch = fgetc( input ) ) != EOF ) {
        // Put the character back into the file.
        ungetc( ch, input );

        // ASCII Symbol to write to output.
        int code = readBits( &buffer, input );
        int sym;

        // If the symbol was deemed invalid, close files and return 1.
        if ( code < 0 ) {
            fclose( input );
            fclose( output );
            fprintf( stderr, "Invalid file\n" );
            return INVALID;
        } else {
            sym = codeToSym( code );
        }

        if ( sym < 0 ) {
            fclose( input );
            fclose( output );
            fprintf( stderr, "Invalid file\n" );
            return INVALID;
        } else {
            fputc( sym, output );
        }
    }

    fclose( input );
    fclose( output );

    return EXIT_SUCCESS;
}
