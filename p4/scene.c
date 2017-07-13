/**
    @file scene.c
    @author Brian Morris (bcmorri3)

    The scene.c program contains the functions and logic defined in scene.h to
    allow a program to work with Scene objects.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "scene.h"
#include "model.h"

Scene *makeScene()
{
    // Dynamically allocate the scene and give it a Model array of size 2.
    Scene *s = (Scene *)malloc( sizeof( Scene ) );
    s->mCount = 0;
    s->mCap = RESIZE;
    s->mList = (Model **)malloc( s->mCap * sizeof( Model * ) );
    return s;
}

void freeScene( Scene *s )
{
    // Free any dynamically allocated Models.
    for ( int i = 0; i < s->mCount; i++ ) {
        freeModel( s->mList[i] );
    }
    // Free the Model list.
    free( s->mList );
    // Free the Scene.
    free( s );
}

bool applyToScene(Scene *s, char const *name, void (*f)(double pt[NUM_COORDS], double a, double b),
                  double a, double b)
{
    // Find the Model with the given name and apply the function to it.
    for ( int i = 0; i < s->mCount; i++ ) {
        if ( strcmp( name, s->mList[ i ]->name ) == 0 ) {
            applyToModel( s->mList[ i ], f, a, b );
            // If applied, return true.
            return true;
        }
    }
    // No match, return false.
    return false;
}

bool containsModel( Scene *s, char const *mname )
{
    // See if there's a matching Model.
    for ( int i = 0; i < s->mCount; i++ ) {
        if ( strcmp( mname, s->mList[ i ]->name ) == 0 ) {
            return true;
        }
    }
    // No match.
    return false;
}

void addModel( Scene *s, char const *fname, char const *mname )
{
    // If the Model array is full, reallocate the memory to an array that's 2 times bigger.
    if ( s->mCount == s->mCap ) {
        s->mCap *= RESIZE;
        s->mList = (Model **)realloc( s->mList, s->mCap * sizeof( Model * ) );
    }

    // Load the Model.
    Model *m = loadModel( fname );
    // If it's not NULL, add it to the list.
    if ( m ) {
        strcpy( m->name, mname );
        s->mList[ s->mCount ] = m;
        s->mCount++;
    }
}

void saveScene( Scene *s, char const *fname )
{
    // Open the output file.
    FILE *output = fopen( fname, "w" );
    if ( !output ) {
        fprintf( stderr, "Can't open file: %s\n", fname );
        return;
    }

    // Sort the models.
    sortModels( s );

    // Print the line segments of each Model.
    for ( int i = 0; i < s->mCount; i++ ) {
        Model *m = s->mList[ i ];
        for (int j = 0; j < m->pCount; j++ ) {
            fprintf( output, "%.3lf %.3lf\n", m->pList[ j ][ 0 ], m->pList[ j ][ 1 ] );
            if ( j % 2 == 1 ) {
                fprintf( output, "\n" );
            }
        }
    }
    // Close output file.
    fclose( output );
}

void removeModel( Scene *s, char const *mname )
{
    // Find the matching Model.
    int modelIndex;
    for ( int i = 0; i < s->mCount; i++ ) {
        if ( strcmp( mname, s->mList[ i ]->name ) == 0 ) {
            modelIndex = i;
            break;
        }
    }

    // Free the matching Model.
    freeModel( s->mList[ modelIndex ] );
    for ( int i = modelIndex; i < s->mCount - 1; i++ ){
        s->mList[ i ] = s->mList[ i + 1 ];
    }

    // Decrease the number of Models.
    s->mCount--;
}

void list( Scene *s )
{
    // Sort the models in the Scene.
    sortModels( s );

    // List the information.
    for ( int i = 0; i < s->mCount; i++ ) {
        Model *m = s->mList[ i ];
        printf( "%s %s (%d)\n", m->name, m->fname, m->pCount / 2 );
    }
}

void sortModels( Scene *s )
{
    // Temporary model pointer.
    Model *temp;
    // Temporary name array.
    char tempName[ NAME_LIMIT + 1];
    // The names of all of the Models.
    char names[ s->mCount ][ NAME_LIMIT + 1 ];
    // Store the names of all of the Models.
    for (int i = 0; i < s->mCount; i++ ) {
        strcpy( names[ i ], s->mList[ i ]->name );
    }

    // If name at i is greater than name at j, swap their positions.
    for ( int i = 0; i < s->mCount; i++ ) {
        for ( int j = i + 1; j < s->mCount; j++ ) {
            if ( strcmp( names[ i ], names[ j ] ) > 0 ) {
               temp = s->mList[ i ];
               strcpy( tempName, names[ i ]);
               s->mList[ i ] = s->mList[ j ];
               strcpy( names[ i ], names[ j ]);
               s->mList[ j ] = temp;
               strcpy( names[ j ], tempName );
            }
        }
    }
}

Model *getModel( Scene *s, char const *mname )
{
    // See if there's a matching Model.
    for ( int i = 0; i < s->mCount; i++ ) {
        if ( strcmp( mname, s->mList[ i ]->name ) == 0 ) {
            return s->mList[ i ];
        }
    }
    // No match.
    return NULL;
}

void addModelPointer( Scene *s, Model * const m )
{
    // If the Model array is full, reallocate the memory to an array that's 2 times bigger.
    if ( s->mCount == s->mCap ) {
        s->mCap *= RESIZE;
        s->mList = (Model **)realloc( s->mList, s->mCap * sizeof( Model * ) );
    }

    // If it's not NULL, add it to the list.
    if ( m ) {
        s->mList[ s->mCount ] = m;
        s->mCount++;
    }
}
