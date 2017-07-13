/**
    @file drawing.c
    @author Brian Morris (bcmorri3)

    The drawing program draws a Scene by using input files and commands from
    standard input. Models are loaded from input files and stored within the Scene.
    Once the user is satisfied with how the Scene has been changed, the Scene can be
    saved to an output file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "scene.h"
#include "model.h"

/** The array position of an x-coordinate to a point in a line segment. */
#define X_COORD 0

/** The array position of a y-coordinate to a point in a line segment. */
#define Y_COORD 1

/** The value of pi. */
#define PI acos( -1.0 )

/** The number of degrees that must be used to convert degrees to radians. */
#define DEG_TO_RAD 180

/** The number of valid commands for the program. */
#define NUM_VALID_COMMANDS 10

/** The maximum length of a command string. */
#define MAX_COMMAND_LEN 9

/** The format string used to scan a command string. */
#define SCAN_COMMAND "%10s"

/** The maximum length of a model name or file name. */
#define NAME_LEN 20

/** The format string used to scan the parameters of the load command. */
#define SCAN_LOAD "%*s%21s%21s"

/** The format string used to scan the parameters of the save command. */
#define SCAN_SAVE "%*s%21s"

/** The format string used to scan the parameters of the delete command. */
#define SCAN_DELETE "%*s%21s"

/** The format string used to scan the parameters of the translate command. */
#define SCAN_TRANSLATE "%*s%21s%lf%lf"

/** The format string used to scan the parameters of the scale command. */
#define SCAN_SCALE "%*s%21s%lf"

/** The format string used to scan the parameters of the rotate command. */
#define SCAN_ROTATE "%*s%21s%lf"

/** The format string used to scan the parameters of the copy command. */
#define SCAN_COPY "%*s%21s%21s"

/** The format string used to scan the parameters of the merge command. */
#define SCAN_MERGE "%*s%21s%21s%21s"

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

/** The length of the string used to parse parameters. */
#define PARAM_LEN 1000

/** The number of matches that fscanf must find for a command with 1 parameter to be valid. */
#define ONE_REQUIRED 1

/** The number of matches that fscanf must find for a command with 2 parameters to be valid. */
#define TWO_REQUIRED 2

/** The number of matches that fscanf must find for a command with 3 parameters to be valid. */
#define THREE_REQUIRED 3

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
    @param b a second double parameter for other functions that need two parameters, unused here.
 */
void scale( double pt[ NUM_COORDS ], double a, double b )
{
    pt[ X_COORD ] *= a;
    pt[ Y_COORD ] *= a;
}

/**
    The rotate function rotates a point by the given angle a using the rotation formula commonly
    used in linear algebra and trigonometry.

    @param pt the point to rotate.
    @param a the angle of the rotation.
    @param b a second double parameter for other functions that need two parameters, unused here.
 */
void rotate( double pt[ NUM_COORDS ], double a, double b )
{
    // Temporary x and y variables as rotation depends on the original x and y values.
    double x = pt[ X_COORD ];
    double y = pt[ Y_COORD ];

    pt[ X_COORD ] = x * cos( a * PI / DEG_TO_RAD ) - y * sin( a * PI / DEG_TO_RAD );
    pt[ Y_COORD ] = x * sin( a * PI / DEG_TO_RAD ) + y * cos( a * PI / DEG_TO_RAD );
}

/**
    The flushInput function flushes standard input until a newline character or EOF
    is found. If EOF is found, the program is exited.

    @param s the Scene to free if EOF is encountered.
 */
void flushInput( Scene *s )
{
    int ch;
    // Clear buffer until new line.
    while ( ( ch = getchar() ) != '\n' ) {
        // If EOF is encountered, free the scene and exit the program.
        if ( ch == EOF ) {
            freeScene( s );
            exit( EXIT_SUCCESS );
        }
    }
}

/**
    The getValidCommand function returns the array index of a valid command if the given string
    is a valid command. If the command is invalid -1 is returned instead.

    @param commmand the command that needs to be validated.

    @return The array index of the valid command, or -1 if invalid.
 */
int getValidCommand( char const * command )
{
    // Valid command list.
    char *validCommands[] = { "load", "save", "delete", "list", "translate", "scale", "rotate",
                              "quit", "copy", "merge" };
    // Find a match.
    for ( int i = 0; i < NUM_VALID_COMMANDS; i++ ) {
        if ( strcmp( command, validCommands[ i ] ) == 0 ) {
            return i;
        }
    }

    // No match found.
    return INVALID_COMMAND;
}

/**
    The loadCommand function loads a Model into the given scene. If the command parameters are
    invalid, an error message is output and the Model is not added to the scene.

    @param s the Scene to add a model to.
    @param commandNum the number of the command that issued a model to be loaded.
    @param params the input string that contains the parameters for loading a model.
 */
void loadCommand( Scene *s, int commandNum, char const * params )
{
    // Array for Model name.
    char modelName[ NAME_LEN + 2 ];
    // Array for file name.
    char fileName[ NAME_LEN + 2 ];
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If there are not two matches, print error message.
    if ( fscanf( sparams, SCAN_LOAD, modelName, fileName ) != TWO_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If there is additional input after the second parameter.
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the name of the Model or file name is too long, print error message.
    if ( strlen( modelName ) > NAME_LEN || strlen( fileName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Determine if the scene already contains the model.
    bool hasModel = containsModel( s, modelName );

    // If the model is already contained within the scene, print error message.
    if ( hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Add the model to the scene.
    addModel( s, fileName, modelName );

    // Close file object.
    fclose( sparams );
}

/**
    The saveCommand function saves the given Scene to an output file. If the command parameters
    are invald, an error message is output and the file is not created.

    @param s the Scene to save to an output file.
    @param commandNum the number of the command that issued the Scene to be saved.
    @param params the input string that contains the parameters to save the Scene.
 */
void saveCommand( Scene *s, int commandNum, char const * params )
{
    // Array for file name.
    char fileName[ NAME_LEN + 2 ];
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If the number of required parameters is not met, print error message.
    if ( fscanf( sparams, SCAN_SAVE, fileName ) != ONE_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If there is trailing input, print error message.
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the file name is too long, print error message.
    if ( strlen( fileName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Save the scene.
    saveScene( s, fileName );

    // Close file object.
    fclose( sparams );
}

/**
    The deleteCommand function deletes a Model from the given Scene. If the command parameters
    are invalid, an error message is output and the Model is not removed from the Scene.

    @param s the Scene to delete a Model from.
    @param commandNum the number of the command that issued a Model to be deleted.
    @param params the input string that contains the parameters to delete a Model.
 */
void deleteCommand( Scene *s, int commandNum, char const * params )
{
    // Array for the model name.
    char modelName[ NAME_LEN + 2 ];

    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If the number of required parameters is not met, print error message.
    if ( fscanf( sparams, SCAN_DELETE, modelName ) != ONE_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If there is trailing input, print error message.
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the Model name is too long, print error message.
    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Determine if the Scene contains the Model.
    bool hasModel = containsModel( s, modelName );

    // If the Model is not found within the Scene, print error message.
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Remove the model from the Scene.
    removeModel( s, modelName );

    // Close the file object.
    fclose( sparams );
}

/**
    The listCommand function displays all of the Models contained within the given Scene. If
    the command parameters are invalid, an error message is output and the Models are not
    displayed.

    @param s the Scene containing the Models to display.
    @param commandNum the number of the command that issued the Models of a Scene to be listed.
    @param params the input string that contains the parameters to list a Scene.
 */
void listCommand( Scene *s, int commandNum, char const * params )
{
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If there is any other input on the params line, print error message.
    if ( fscanf( sparams, "%*s%c", &trailingChar ) == 1 && trailingChar != '\n'
         && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // List the Scene.
    list( s );

    // Close the file object.
    fclose( sparams );
}

/**
    The translateCommand function applies the translate function to a given Model in the given
    Scene. If the Model cannot be found in the Scene or any of the required parameters are invalid,
    an error message is output and the translate function is not applied to the Model.

    @param s the Scene containing the Model to translate.
    @param commandNum the number of the command that issued a Model to be translated.
    @param params the input string that contains the parameters to translate a Model.
 */
void translateCommand( Scene *s, int commandNum, char const * params )
{
    // Array for the Model name.
    char modelName[ NAME_LEN + 2 ];
    // First double value used for translation.
    double a;
    // Second double value used for translation.
    double b;
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If the number of required parameters is not met, print error message.
    if ( fscanf( sparams, SCAN_TRANSLATE, modelName, &a, &b ) != THREE_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If there is trailing input, print error message.
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the Model name is too long, print error message.
    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Determine if the Scene contains the Model.
    bool hasModel = applyToScene( s, modelName, &translate, a, b );
    // If it doesn't have the Model, print error message.
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
    }

    // Close the file object.
    fclose( sparams );
}

/**
    The scaleCommand function applies the scale function to a given Model in the given Scene.
    If the Model cannot be found in the Scene or any of the required parameters are invalid,
    an error message is output and the scale function is not applied to the Model.

    @param s the Scene containing the Model to scale.
    @param commandNum the number of the command that issued a Model to be scaled.
    @param params the input string that contains the parameters to scale a Model.
 */
void scaleCommand( Scene *s, int commandNum, char const * params )
{
    // Array for the Model name.
    char modelName[ NAME_LEN + 2 ];
    // The scaling factor of the scale command.
    double a;
    // Dummy variable for second parameter.
    double b = 0;
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If the input line does not contain the number of required parameters, print error message.
    if ( fscanf( sparams, SCAN_SCALE, modelName, &a ) != TWO_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the input line contains any trailing input, print error message.
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the Model name is too loing, print error message.
    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Determine if the given Scene contains the Model.
    bool hasModel = applyToScene( s, modelName, &scale, a, b );

    // If it doesn't contain the Model, print error message.
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
    }

    // Close file object.
    fclose( sparams );
}

/**
    The rotateCommand function applies the rotate function to a given Model in the given Scene.
    If the Model cannot be found in the Scene or any of the required parameters are invalid,
    an error message is output and the rotate function is not applied to the Model.

    @param s the Scene containing the Model to rotate.
    @param commandNum the number of the command that issued a Model to be rotated.
    @param params the input string that contains the parameters to rotate a Model.
 */
void rotateCommand( Scene *s, int commandNum, char const * params )
{
    // Array for the Model name.
    char modelName[ NAME_LEN + 2 ];
    // The angle to rotate the Model by.
    double a;
    // Dummy variable for second parameter.
    double b = 0;

    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If the required number of parameters is not found, print error message.
    if ( fscanf( sparams, SCAN_ROTATE, modelName, &a ) != TWO_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If there is any trailing input, print error message.
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the Model name is too long, print error messsage.
    if ( strlen( modelName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Determine if the Scene already contains the Model.
    bool hasModel = applyToScene( s, modelName, &rotate, a, b );

    // If it does not have the Model, print error message.
    if ( !hasModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
    }

    // Close file object.
    fclose( sparams );
}

/**
    The quitCommand function frees the given Scene from memory and exits the program.
    If there is any trailing input after the command was issued, the command is considered
    invalid and an error message is output.

    @param s the Scene to free.
    @param commandNum the number of the command that issued the program to quit.
    @param params the input string that contains the parameters to quit the program.
 */
void quitCommand( Scene *s, int commandNum, char const * params )
{
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If there is any trailing input, output error message.
    if ( fscanf( sparams, "%*s%c", &trailingChar ) == 1 && trailingChar != '\n'
         && trailingChar != EOF && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Free the Scene.
    freeScene( s );
    // Close file object.
    fclose( sparams );
    // Exit the program.
    exit( EXIT_SUCCESS );
}

/**
    The copyCommand function copies a Model found in the given Scene and adds a duplicate Model
    to the Scene with a different name. If the source Model cannot be found in the scene, any of
    the copy parameters are invalid, or there is any trailing input after the parameters, the
    command is considered invalid and an error message is output.

    @param s the Scene to copy a Model from.
    @param commandNum the number of the command that issued a Model to be copied.
    @param params the input string containing the parameters for the copy command.
 */
void copyCommand( Scene *s, int commandNum, char const * params )
{
    // Array for destination Model name.
    char destName[ NAME_LEN + 2 ];
    // Array for source Model name.
    char sourceName[ NAME_LEN + 2 ];
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If there are not two matches, print error message.
    if ( fscanf( sparams, SCAN_COPY, destName, sourceName ) != TWO_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If there is additional input after the second parameter
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the name of the destination or source is too long, print error message.
    if ( strlen( destName ) > NAME_LEN || strlen( sourceName ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Determine if the Scene already contains the destination Model.
    bool hasDestModel = containsModel( s, destName );
    // Determne if the Scene contains the source Model.
    bool hasSourceModel = containsModel( s, sourceName );

    // If the destination model is already contained within the Scene, or the source Model does not
    // exist in the scene, print error message.
    if ( hasDestModel || !hasSourceModel ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Get the source Model.
    Model *sourceModel = getModel( s, sourceName );
    if ( !sourceModel ) {
        fclose( sparams );
        return;
    } else {
        addModel( s, sourceModel->fname, destName );
    }

    // Close file object.
    fclose( sparams );
}

/**
    The mergeCommand function merges two Models found in the given Scene into one Model with a
    different name. If the source Models cannot be found in the scene, any of
    the merge parameters are invalid, or there is any trailing input after the parameters, the
    command is considered invalid and an error message is output.

    @param s the Scene containing the Models to merge.
    @param commandNum the number of the command that issued Models to be merged.
    @param params the input string containing the parameters for the merge command.
 */
void mergeCommand( Scene *s, int commandNum, char const * params )
{
    // Array for destination Model name.
    char destName[ NAME_LEN + 2 ];
    // Array for first source Model name.
    char sourceName1[ NAME_LEN + 2 ];
    //Array for second source Model name.
    char sourceName2[ NAME_LEN + 2 ];
    // Char to determine if there is any input remaining on the input line.
    char trailingChar;

    // Treat params as a file to ensure that the input cursor is moved properly.
    FILE *sparams = fmemopen( (void *)params, strlen( params ), "r" );

    // If there are not two matches, print error message.
    if ( fscanf( sparams, SCAN_MERGE, destName, sourceName1, sourceName2 ) != THREE_REQUIRED ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If there is additional input after the third parameter
    if ( fscanf( sparams, "%c", &trailingChar ) == 1 && trailingChar != '\n' && trailingChar != EOF
         && trailingChar != '\0' ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // If the name of the Models are too long, print error message.
    if ( strlen( destName ) > NAME_LEN || strlen( sourceName1 ) > NAME_LEN
         || strlen( sourceName2 ) > NAME_LEN ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Determine if the Scene already contains the destination Model.
    bool hasDestModel = containsModel( s, destName );
    // Determne if the scene contains the first source Model.
    bool hasSourceModel1 = containsModel( s, sourceName1 );
    // Determine if the Scene contains the second source Model.
    bool hasSourceModel2 = containsModel( s, sourceName2 );

    // If the destination model is already contained within the Scene, or the source Models do not
    // exist in the scene, print error message.
    if ( hasDestModel || !hasSourceModel1 || !hasSourceModel2 ) {
        fprintf( stderr, "Command %d invalid\n", commandNum );
        fclose( sparams );
        return;
    }

    // Retrieve the source Models.
    Model *sourceModel1 = getModel( s, sourceName1 );
    Model *sourceModel2 = getModel( s, sourceName2 );
    if ( !sourceModel1 || !sourceModel2 ) {
        fclose( sparams );
        return;
    } else {
        // Merge the models.
        Model *m = mergeModels( sourceModel1, sourceModel2 );
        strcpy( m->name, destName );

        //Add the merged Model to the list.
        addModelPointer( s, m );
        // Remove source Models.
        removeModel( s, sourceName1 );
        removeModel( s, sourceName2 );
    }

    // Close file object.
    fclose( sparams );
}

/**
    The main function handles getting input from the user, passing the responsibility
    of error checking to the other functions. If the command input is invalid, an error
    message is output and the user is reprompted for a new command. If EOF is encountered
    or the user decides to quit the program, a successful exit status is returned.

    @return Program exit status.
 */
int main()
{
    // Create an empty Scene.
    Scene *s = makeScene();

    // Command number counter.
    int commandNum = 1;

    // Array to store a command.
    char command[ MAX_COMMAND_LEN + 2 ];

    // Array to store parameter lines.
    char params[ PARAM_LEN + 1 ];
    // Ensure the string is null terminated.
    for ( int i = 0; i <= PARAM_LEN; i++ ) {
        params[ i ] = '\0';
    }

    // Prompt for commands.
    printf( "cmd %d> ", commandNum );
    // Prompt until EOF.
    while ( fgets( params, sizeof( params ), stdin ) ) {
        // If the param line is longer than 1000 characters, flush remaining input.
        if ( params[ strlen( params ) - 1 ] != '\n' ) {
            flushInput( s );
        }

        // Parse command value.
        sscanf( params, SCAN_COMMAND, command );
        // The index of the command.
        int commandIndex = getValidCommand( command );

        // If the command is valid, determine which function to call.
        if ( commandIndex != INVALID_COMMAND ) {
            switch ( commandIndex ) {
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
            // Print error message.
            fprintf( stderr, "Command %d invalid\n", commandNum );
        }
        // Reprompt.
        printf( "cmd %d> ", ++commandNum );
    }

    // Free the Scene.
    freeScene( s );
    // Return success.
    return EXIT_SUCCESS;
}
