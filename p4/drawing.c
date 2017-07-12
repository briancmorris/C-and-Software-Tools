#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "scene.h"
#include "model.h"

/** The array position of an x-coordinate to a point in a line segment. */
#define X_COORD 0

/** The array positio of a y-coordinate to a point in a line segment. */
#define Y_COORD 1

/** The value of pi. */
#define PI acos( -1.0 )

/** The number of degrees that must be used to convert degrees to radians. */
#define DEG_TO_RAD 180

/** The number of valid commands for the progam. */
#define NUM_VALID_COMMANDS 10

/** The maximum length of a command string. */
#define MAX_COMMAND_LEN 9

/** The format string used to scan a command string. */
#define SCAN_COMMAND "%10s"

/** The maximum length of a model name or file name. */
#define NAME_LEN 20

/** The format string used to scan the paramaters of the load command. */
#define SCAN_LOAD "%*s%21s%21s"

/** The format string used to scan the parameters of the save command. */
#define SCAN_SAVE "%*s%21s"

/** The format string used to scan the parameters of the delete command. */
#define SCAN_DELETE "%*s%21s"

/** The format string used to scan the parameters of the translate command. */
#define SCAN_TRANSLATE "%*s%21s%lf%lf"

/** The format string used to scan the paramaters of the scale command. */
#define SCAN_SCALE "%*s%21s%lf"

/** The format string used to scan the paramaters of the rotate command. */
#define SCAN_ROTATE "%*s%21s%lf"

/** The index value of an invalid command. */
#define INVALID_COMMAND -1

/** The index value of the load command. */
#define LOAD_COMMAND 0

/** The index value of the save command. */
#define SAVE_COMMAND 1

/** The index value of the delete command. */
#define DELETE_COMMAND 2

/** The index value of the list command. */
#define LIST_COMMAND 3

/** The index value of the translate command. */
#define TRANSLATE_COMMAND 4

/** The index value of the scale command. */ 
#define SCALE_COMMAND 5

/** The index value of the rotate command. */
#define ROTATE_COMMAND 6

/** The index value of the quit command. */
#define QUIT_COMMAND 7

/** The index value of the copy command. */
#define COPY_COMMAND 8

/** The index value of the merge command. */
#define MERGE_COMMAND 9

/** The length of the string used to parse paramaters. */
#define PARAM_LEN 100

/**
    The translate function translates a point by adding the given value a to the x-coordinate
    of a point and the value b to the y-coordinate of a point.

    @param pt the point to translate.
    @param a the value to translate the x-coordinate by.
    @param b the value to translate the y-coordinate by.
 */
void translate( double pt[ NUM_COORDS ], double a, double b )
{
    pt[ X_COORD ] += a;
    pt[ Y_COORD ] += b;
}

/**
    The scale function scales a point by multiplying both the x-coordinate and y-coordinate of
    a point by the value of a.

    @param pt the point to scale.
    @param a the scaling factor of the point.
    @param b a second double paramater for other functions that need two paramaters, unused here.
 */
void scale( double pt[ NUM_COORDS ], double a, double b )
{
    pt[ X_COORD ] *= a;
    pt[ Y_COORD ] *= a;
}

/**
    The rotate function rotates a point by the 

    @param pt the point to rotate.
    @param a the scaling value of the point.
    @param b a second double paramater for other functions that need two paramaters, unused here.
 */
void rotate( double pt[ NUM_COORDS ], double a, double b )
{
    double originalX = pt[ X_COORD ];
    double originalY = pt[ Y_COORD ];

    pt[ X_COORD ] = originalX * cos( a * PI / DEG_TO_RAD ) - originalY * sin( a * PI / DEG_TO_RAD );
    pt[ Y_COORD ] = originalX * sin( a * PI / DEG_TO_RAD ) + originalY * cos( a * PI / DEG_TO_RAD );
}

void flushInput( Scene *s )
{
    int ch;
    while( ( ch = getchar() ) != '\n' ) {
        if( ch == EOF ) {
            freeScene( s );
            exit( EXIT_SUCCESS );
        }
    }
}

int getValidCommand( char const * command )
{
    char *validCommands[] = { "load", "save", "delete", "list", "translate", "scale", "rotate",
                              "quit", "copy", "merge" };
    for ( int i = 0; i < NUM_VALID_COMMANDS; i++ ) {
        if ( strcmp( command, validCommands[ i ] ) == 0 ) {
            return i;
        }
    }
    return INVALID_COMMAND;
}

void loadCommand( Scene *s, int commandNum, char const * params )
{
    char modelName[ NAME_LEN + 2 ];
    char fileName[ NAME_LEN + 2 ];
    char trailingChar;
    
    FILE *sparams = fmemopen((void *)params, strlen(params), "r");
    
    if ( fscanf( sparams, SCAN_LOAD, modelName, fileName ) != 2 ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose(sparams);
        return;
    }

    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose(sparams);
        return;
    }

    if ( strlen( modelName ) > NAME_LEN || strlen( fileName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose(sparams);
        return;
    }

    bool hasModel = containsModel( s, modelName );
    if ( hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose(sparams);
        return;
    }

    addModel( s, fileName, modelName );
    fclose(sparams);
}

void saveCommand( Scene *s, int commandNum, char const * params )
{
    char fileName[ NAME_LEN + 2 ];
    //char trailingChar;

    if ( sscanf( params, SCAN_SAVE, fileName ) != 1 ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    /*if ( sscanf( params, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }*/

    if ( strlen( fileName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    saveScene( s, fileName );
}

void deleteCommand( Scene *s, int commandNum, char const * params )
{
    char modelName[ NAME_LEN + 2 ];
    //char trailingChar;

    if ( sscanf( params, SCAN_DELETE, modelName ) != 1 ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    /*if ( sscanf( params, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }*/

    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    bool hasModel = containsModel( s, modelName );
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    removeModel( s, modelName );
}

void listCommand( Scene *s, int commandNum, char const * params )
{
    //char trailingChar;
    /*if ( sscanf( params, "%*s%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }*/

    list( s );
}

void translateCommand( Scene *s, int commandNum, char const * params )
{
    char modelName[ NAME_LEN + 2 ];
    double a;
    double b;
    //char trailingChar;

    if ( sscanf( params, SCAN_TRANSLATE, modelName, &a, &b ) != 3 ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    /*if ( sscanf( params, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }*/

    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    bool hasModel = applyToScene( s, modelName, &translate, a, b );
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }
}

void scaleCommand( Scene *s, int commandNum, char const * params )
{
    char modelName[ NAME_LEN + 2 ];
    double a;
    double b = 0;
    //char trailingChar;

    if ( sscanf( params, SCAN_SCALE, modelName, &a ) != 2 ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    /*if ( sscanf( params, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }*/

    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    bool hasModel = applyToScene( s, modelName, &scale, a, b );
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }
}

void rotateCommand( Scene *s, int commandNum, char const * params )
{
    char modelName[ NAME_LEN + 2 ];
    double a;
    double b = 0;
    //char trailingChar;


    if ( sscanf( params, SCAN_ROTATE, modelName, &a ) != 2 ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    /*if ( sscanf( params, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }*/

    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }

    bool hasModel = applyToScene( s, modelName, &rotate, a, b );
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }
}

void quitCommand( Scene *s, int commandNum, char const * params )
{
    //char trailingChar;
    /*if ( sscanf( params, "%*s%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        return;
    }*/
    freeScene( s );
    exit( EXIT_SUCCESS );
}

void copyCommand( Scene *s, int commandNum, char const * params )
{
    
}

void mergeCommand( Scene *s, int commandNum, char const * params )
{
    
}

int main()
{
    Scene *s = makeScene();

    int commandNum = 1;
    char command[ MAX_COMMAND_LEN + 2 ];
    char params[ PARAM_LEN + 1 ];
    for ( int i = 0; i <= PARAM_LEN; i++ ) {
        params[ i ] = '\0';
    }

    printf( "cmd %d> ", commandNum );
    while ( fgets( params, sizeof( params ), stdin ) ) {
        if( params[ strlen( params ) - 1 ] != '\n' ) {
            flushInput( s );
        }

        sscanf( params, SCAN_COMMAND, command );
        int commandIndex = getValidCommand( command );

        if( commandIndex != INVALID_COMMAND ) {
            switch( commandIndex ) {
                case LOAD_COMMAND:
                    loadCommand( s, commandNum, params );
                    break;
                case SAVE_COMMAND:
                    saveCommand( s, commandNum, params );
                    break;
                case DELETE_COMMAND:
                    deleteCommand( s, commandNum, params );
                    break;
                case LIST_COMMAND:
                    listCommand( s, commandNum, params );
                    break;
                case TRANSLATE_COMMAND:
                    translateCommand( s, commandNum, params );
                    break;
                case SCALE_COMMAND:
                    scaleCommand( s, commandNum, params );
                    break;
                case ROTATE_COMMAND:
                    rotateCommand( s, commandNum, params );
                    break;
                case QUIT_COMMAND:
                    quitCommand( s, commandNum, params );
                    break;
                 case COPY_COMMAND:
                    copyCommand( s, commandNum, params );
                    break;
                case MERGE_COMMAND:
                    mergeCommand( s, commandNum, params );
                    break;
                default:
                    break;
            }
        } else {
            fprintf( stderr, "Command %d invalid\n", commandNum );
            //flushInput();
        }

        printf( "cmd %d> ", ++commandNum );
    }

    freeScene( s );
    return EXIT_SUCCESS;
}
