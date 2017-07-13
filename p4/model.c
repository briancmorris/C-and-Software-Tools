/**
    @file model.c
    @author Brian Morris (bcmorri3)

    The model.c program contains the functions and logic defined in model.h to
    allow a program to work with Model objects.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "model.h"

Model *loadModel( char const *fname )
{
    // Open the input file.
    FILE *lineCounter = fopen( fname, "r" );

    // If it's NULL, print error message.
    if ( !lineCounter ) {
        fprintf( stderr, "Can't open file: %s\n", fname );
        return NULL;
    }

    // Used to store x-coordinate of a point.
    double x;
    // Used to store y-coordinate of a point.
    double y;
    // The number of points found within the input file.
    int numPoints = 0;

    // Find the number of points in the input file.
    int matches = fscanf( lineCounter, "%lf %lf", &x, &y );
    while ( matches == 2 ) {
        numPoints++;
        matches = fscanf( lineCounter, "%lf %lf", &x, &y );
    }

    // If there are no points, or an incomplete line segment, print error message.
    if ( numPoints == 0 || numPoints % 2 != 0 ) {
        fprintf( stderr, "Invalid model format: %s\n", fname );
        fclose( lineCounter );
        return NULL;
    }
    fclose( lineCounter );

    // Dynamically allocate the Model.
    Model *m = (Model *)malloc( sizeof( Model ) );
    m->pList = (double (*)[ NUM_COORDS ])malloc( numPoints * NUM_COORDS * sizeof( double ) );
    m->pCount = numPoints;
    strcpy( m->fname, fname );

    // Re-read input file and store its contents in the Model.
    FILE *input = fopen( fname, "r" );

    int count = 0;
    while ( fscanf( input, "%lf %lf", &x, &y ) == 2 && count < numPoints ) {
        m->pList[ count ][ 0 ] = x;
        m->pList[ count ][ 1 ] = y;
        count++;
    }
    fclose( input );

    // Return the model.
    return m;
}

void freeModel( Model *m )
{
    // Free its points.
    free( m->pList );
    // Free the model pointer.
    free( m );
}

void applyToModel( Model *m, void (*f)( double pt[ NUM_COORDS ], double a, double b ), double a,
                   double b )
{
    // For every point in the Model, apply the function f.
    for ( int i = 0; i < m->pCount; i++ ) {
        f( m->pList[ i ], a, b );
    }
}

Model *mergeModels( Model * const sourceModel1, Model * const sourceModel2 )
{
    // Determine the number of points in the merged Model.
    int numPoints = sourceModel1->pCount + sourceModel2->pCount;
    // Dynamically allocate the merged Model.
    Model *m = (Model *)malloc( sizeof( Model ) );
    m->pList = (double (*)[ NUM_COORDS ])malloc( numPoints * NUM_COORDS * sizeof( double ) );
    m->pCount = numPoints;
    strcpy( m->fname, "-" );

    // Counter for merged Model pList index.
    int count = 0;

    // Give it the points of sourceModel1.
    for ( int i = 0; i < sourceModel1->pCount; i++ ) {
        m->pList[ count ][ 0 ] = sourceModel1->pList[ i ][ 0 ];
        m->pList[ count ][ 1 ] = sourceModel1->pList[ i ][ 1 ];
        count++;
    }
    // Give it the points of sourceModel2.
    for ( int i = 0; i < sourceModel2->pCount; i++ ) {
        m->pList[ count ][ 0 ] = sourceModel2->pList[ i ][ 0 ];
        m->pList[ count ][ 1 ] = sourceModel2->pList[ i ][ 1 ];
        count++;
    }

    // Return the merged model.
    return m;
}

Model *copyModel( Model * const sourceModel )
{
    // Determine the number of points in the copied Model.
    int numPoints = sourceModel->pCount;

    // Dynamically allocate the copied Model.
    Model *m = (Model *)malloc( sizeof( Model ) );
    m->pList = (double (*)[ NUM_COORDS ])malloc( numPoints * NUM_COORDS * sizeof( double ) );
    m->pCount = numPoints;
    strcpy( m->fname, sourceModel->fname );

    // Give it the points of the source Model.
    for ( int i = 0; i < numPoints; i++ ) {
        m->pList[ i ][ 0 ] = sourceModel->pList[ i ][ 0 ];
        m->pList[ i ][ 1 ] = sourceModel->pList[ i ][ 1 ];
    }

    // Return the duplicate.
    return m;
}
