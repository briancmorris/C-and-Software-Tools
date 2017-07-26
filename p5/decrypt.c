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

//    while ( ) {
//    }

    fclose( input );
    fclose( output );

    return EXIT_SUCCESS;
}
