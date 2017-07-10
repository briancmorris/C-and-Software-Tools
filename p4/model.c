#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "model.h"

Model *loadModel( char const *fname )
{
    FILE *lineCounter = fopen( fname, "r" );
    if ( !lineCounter ) {
        fprintf( stderr, "Can't open file: %s\n", fname );
        return NULL;
    }

    double x;
    double y;
    int numPoints = 0;

    int matches = fscanf( lineCounter, "%lf %lf", &x, &y );
    while ( matches == 2 ) {
        numPoints++;
        matches = fscanf( lineCounter, "%lf %lf", &x, &y );
    }

    if( numPoints == 0 || numPoints % 2 != 0 ) {
        fprintf( stderr, "Invalid model format: %s\n", fname );
        fclose( lineCounter );
        return NULL;
    }
    fclose( lineCounter );

    Model *m = (Model *)malloc( sizeof( Model ) );
    m->pList = (double (*)[ NUM_COORDS ])malloc( numPoints * NUM_COORDS * sizeof( double ) );
    m->pCount = numPoints;
    strcpy( m->fname, fname );

    FILE *input = fopen( fname, "r" );

    int count = 0;
    while ( fscanf( input, "%lf %lf", &x, &y ) == 2 && count < numPoints ) {
        m->pList[ count ][ 0 ] = x;
        m->pList[ count ][ 1 ] = y;
        count++;
    }
    fclose( input );

    return m;
}

void freeModel( Model *m )
{
    free( m->pList );
    free( m );
}

void applyToModel( Model *m, void (*f)( double pt[ NUM_COORDS ], double a, double b ), double a,
                   double b )
{
    for( int i = 0; i < m->pCount; i++ ) {
        f( m->pList[ i ], a, b );
    }
}
