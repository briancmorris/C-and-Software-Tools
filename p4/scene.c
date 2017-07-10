#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "scene.h"
#include "model.h"

Scene *makeScene()
{
    Scene *s = (Scene *)malloc( sizeof( Scene ) );
    s->mCount = 0;
    s->mCap = RESIZE;
    s->mList = (Model **)malloc( s->mCap * sizeof( Model * ) );
    return s;
}

void freeScene( Scene *s )
{
    for ( int i = 0; i < s->mCount; i++ ) {
        freeModel( s->mList[i] );
        //free( s->mList[i] );
    }
    //for ( int i = s->mCount; i < s->mCap; i++ ) {
    //    free( s->mList[i] );
    //}
    free( s->mList );
    free( s );
}

bool applyToScene(Scene *s, char const *name, void (*f)(double pt[NUM_COORDS], double a, double b),
                  double a, double b)
{
    for ( int i = 0; i < s->mCount; i++ ) {
        if ( strcmp( name, s->mList[ i ]->name ) == 0 ) {
            applyToModel( s->mList[ i ], f, a, b );
            return true;
        }
    }
    return false;
}

bool containsModel( Scene *s, char const *name )
{
    for ( int i = 0; i < s->mCount; i++ ) {
        if ( strcmp( name, s->mList[ i ]->name ) == 0 ) {
            return true;
        }
    }
    return false;
}

void addModel( Scene *s, char const *fname, char const *mname )
{
    if ( s->mCount == s->mCap ) {
        s->mCap *= RESIZE;
        s->mList = (Model **)realloc( s->mList, s->mCap * sizeof( Model * ) );
    }

    Model *m = loadModel( fname );
    if ( m ) {
        strcpy( m->name, mname );
        s->mList[ s->mCount ] = m;
        s->mCount++;
    }
}

void saveScene( Scene *s, char const *fname )
{
    FILE *output = fopen( fname, "w" );
    if ( !output ) {
        fprintf( stderr, "Can't open file: %s\n", fname );
        return;
    }

    sortModels( s );

    for ( int i = 0; i < s->mCount; i++ ) {
        Model *m = s->mList[ i ];
        for(int j = 0; j < m->pCount; j++ ) {
            fprintf( output, "%.3lf %.3lf\n", m->pList[ j ][ 0 ], m->pList[ j ][ 1 ] );
            if ( j % 2 == 1 ) {
                fprintf( output, "\n" );
            }
        }
    }
    fclose( output );
}

void removeModel( Scene *s, char const *mname )
{
    int modelIndex;
    for ( int i = 0; i < s->mCount; i++ ) {
        if ( strcmp( mname, s->mList[ i ]->name ) == 0 ) {
            modelIndex = i;
            break;
        }
    }

    //int numModelsShift = s->mCap - modelIndex - 1;
    freeModel( s->mList[ modelIndex ] );
    for(int i = modelIndex; i < s->mCount - 1; i++){
        s->mList[i] = s->mList[i+1];
    }
    //memmove( s->mList[ modelIndex ], s->mList[ modelIndex + 1 ], numModelsShift * sizeof( Model * ) );
    s->mCount--;
}

void list( Scene *s )
{
    sortModels( s );
    for ( int i = 0; i < s->mCount; i++ ) {
        Model *m = s->mList[ i ];
        printf( "%s %s (%d)\n", m->name, m->fname, m->pCount / 2 );
    }
}

void sortModels( Scene *s )
{
    Model *temp;
    char tempName[ NAME_LIMIT + 1];
    char names[ s->mCount ][ NAME_LIMIT + 1 ];
    for (int i = 0; i < s->mCount; i++ ) {
        strcpy( names[ i ], s->mList[ i ]->name );
    }

    for( int i = 0; i < s->mCount; i++ ) {
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
