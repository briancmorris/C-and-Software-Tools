/**
    @file jumble.c
    @author Brian Morris (bcmorri3)
    The jumble program accepts an file name from command line arguments and stores
    the words found within the file in a two-dimensional array that can be searched
    for with user input. The program allows the user to find all words contained within the
    file that have the exact characters specified by the user. If any file errors occur
    or the number of command line arguments are invalid, the program is exited with a status of 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** The maximum number of words the input file can contain. */
#define MAX_WORDS 100000

/** The maximum number of characters a word can contain including the null terminator. */
#define MAX_CHARS 21

/** The exit status of the program in the event of an error or invalid input. */
#define ERROR_STATUS 1

/** The number of valid command line arguments that must be input to run the program. */
#define VALID_ARGS 2

/** The position of the file name found within the command line arguments array. */
#define FILE_NAME_ARGS 1

/** The length of a temporary array used to store and check input for validity. */
#define TEMP_SIZE 50


/** The two-dimensional array of characters used to store the words found in the input file. */
static char words[ MAX_WORDS ][ MAX_CHARS ];

/** The number of words found within the input file. */
int wordCount;

bool matches( char const *word, char const *letters )
{
    // Length of both char arrays.
    int wordLen = strlen( word );
    int lettersLen = strlen( letters );

    // If the length of the word and user input don't match, return false.
    if ( wordLen != lettersLen ) {
        return false;
    }

    // Create temp char array containing the contents of word.
    int tempLen = wordLen;
    char temp[ tempLen ];
    for ( int i = 0; i < tempLen; i++ ) {
        temp[ i ] = *( word + i );
    }

    // Compare content of letters array to contents of temp. If there is a match, replace
    // temp contents with a space ' '.
    for ( int i = 0; i < lettersLen; i++ ) {
        for ( int j = 0; j < tempLen; j++ ) {
            if ( *( letters + i ) == temp[ j ] ) {
                temp[ j ] = ' ';
                break; // Check next char in letters, replace only one char in temp.
            }
        }
    }
    
    // Make sure the contents of temp are all spaces.
    for ( int i = 0; i < tempLen; i++ ) {
        if ( temp[ i ] != ' ' ) {
            return false;
        }
    }

    // There's a match because all chars in temp are spaces.
    return true;
}

/**
    The get letters function continuously prompts the user for letters until valid input
    is provided or end of file is encountered. If valid input is provided, the function
    returns true. If end of file is encountered, the function returns false. Input is
    considered valid if the input string consists of all lowercase letters that are up to
    20 characters in length.
    @param letters a char array used to store valid input.
    @return true if the user inputs valid letters, false if EOF is encountered.
 */
bool getLetters( char *letters )
{
    // Length of input string.
    int len;
    // Temp array used for comparison before storing in letters array.
    char temp[ TEMP_SIZE ];
    // True if the user inputs valid letters.
    bool validLetters;

    // Initial prompt.
    printf( "letters> ");

    // Continuously prompt if needed.
    while ( scanf( "%s", temp ) == 1) {
        // Account for null terminator.
        len = strlen( temp ) + 1;

        // If the number of characters input is invalid, reprompt user.
        if ( len > MAX_CHARS ) {
            printf( "Invalid letters\n" );
            printf( "letters> ");
        } else {
            // Determine if user input is composed of valid characters.
            for (int i = 0; i < len; i++ ) {
                // If invalid char is found, set validLetters to false and break out of for loop.
                if ( ( temp[ i ] < 'a' || temp[ i ] > 'z' ) && temp[ i ] != '\0' ) {
                    validLetters = false;
                    break;
                } else {
                    validLetters = true;
                }
            }

            // If input is valid, store in letters array and return true.
            if ( validLetters ) {
                for (int i = 0; i < len; i++ ) {
                    *( letters + i ) = temp[ i ];
                }
                //printf( "Valid letters are: %s\n", letters );
                return true;
            } else {
                // Reprompt user.
                printf( "Invalid letters\n" );
                printf( "letters> " );
            }
        }
    }

    // EOF has been encountered, print new line and return false.
    return false;

    // Consider shadowing.
    // Consider nested while loops EOF '\n'.
    /*
        while ( ( ch = getchar() ) != EOF ) {
        // If the number of characters input is too many, reprompt user.
        if ( charCount >= MAX_CHARS ) {
            printf( "Invalid letters\nletters> " );
            charCount = 0;
        } else if ( ch == '\n') { // Handle new line.
            if ( charCount == 0 ) {
                printf( "Invalid letters\nletters> ");
            } else {
                break;
            }
        } else if ( ch < 'a' || ch > 'z' ) { // Make sure letters are valid.
            printf( "Invalid letters\nletters> ");
            charCount = 0;
        } else { // Add to temporary array.
            temp[ charCount++ ] = ch;
        }
    }
    
    if ( ch == '\n' && charCount < MAX_CHARS ) {
        
    }*/
}

/**
    The readWords function stores the words found in the input file with the given file name
    and stores them in the two-dimensional words array. If there is an issue with opening the file
    or any of the words found within the file are invalid, the program is exited with a status
    of 1.
    @param filename the name of the input file
 */
void readWords( char const *filename )
{
    FILE *input = fopen( filename, "r" );

    // If the file can't be opened.
    if ( input == NULL ) {
        fprintf( stderr, "Can't open word file\n" );
        fclose ( input );
        exit( ERROR_STATUS );
    }

    wordCount = 0;
    int len;
    char temp[ TEMP_SIZE ];

    // While scanf has a string match.
    while ( fscanf( input, "%s", temp ) == 1 ) {
        //printf( "The temporary word is: %s\n", temp );
        
        // Handle too many words.
        if ( wordCount >= MAX_WORDS ) {
            goto invalid;
        }
        //printf( "%s survived word count\n", temp );
        
        // Handle invalid number of characters.
        len = strlen( temp ) + 1; // Account for null terminator.
        if ( len > MAX_CHARS ) {
            goto invalid;
        }
        //printf( "%s survived str len\n", temp );

        // Check that all characters are lowercase letters or null terminator.
        for ( int i = 0; i < len; i++ ) {
            if ( ( temp[ i ] < 'a' || temp[ i ] > 'z' ) && temp[ i ] != '\0' ) {
                goto invalid;
            } else {
                words[ wordCount ][ i ] = temp[ i ];
            }
        }
        //printf( "%s survived valid characters\n", temp );
        //printf( "%s is stored in the array\n", words[ wordCount ] );
        wordCount++;
    }
    goto valid;

    // Print error message for invalid file format.
    invalid:
    fprintf( stderr, "Invalid word file\n" );
    fclose ( input );
    exit( ERROR_STATUS );

    // Close file if successful.
    valid:
    fclose( input );
}

int main( int argc, char *argv[] )
{
    // Handle invalid number of command line arguments.
    if ( argc != VALID_ARGS ) {
        fprintf( stderr, "usage: jumble <word-file>\n");
        return ERROR_STATUS;
    }

    // Read in words.
    readWords( argv[ FILE_NAME_ARGS ] );

    char letters[ MAX_CHARS ];
    bool prompt = getLetters( letters );

    // Prompt user continuously for input until EOF.
    while ( prompt ) {
        for (int i = 0; i < wordCount; i++ ) {
            bool match = matches( words[ i ], letters );
            if ( match ) {
                printf( "%s\n", words[ i ] );
            }
        }

        //printf("Returned True\n");
        prompt = getLetters( letters );
    }
    //printf("Returned False\n");
    return EXIT_SUCCESS;
}
