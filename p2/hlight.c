/**
   @file hlight.c
   @author Brian Morris (bcmorri3)
   This program accepts HTML style input from standard input and outputs the provided
   text with HTML tags highlighted red and HTML entities highlighted blue. If the provided
   input has incomplete tags or entities, the program is exited with an unsuccessful exit status.
 */

#include <stdio.h>
#include <stdlib.h>

/** The opening ANSI escape sequence required to output HTML tags in red. */
#define ANSI_TAG_OPEN "\x1B[31m"

/** The opening ANSI escape sequence required to output HTML entities in blue. */
#define ANSI_ENTITY_OPEN "\x1B[34m"

/** The closing ANSI escape sequence required return output to standard text color. */
#define ANSI_RESET "\x1B[0m"

/** The exit status of the program when an incomplete HTML tag is encountered. */
#define INVALID_TAG 101

/** The exit status of the program when an incomplete HTML entity is encountered. */
#define INVALID_ENTITY 100

/**
   Outputs the HTML tag read in from standard input with red text, or exits the program
   with an exit status of 101 if the end of file character is encountered before the closing
   '>' character.
 */
void showTag()
{
    // Initialize input character variable to compare with EOF character.
    int ch;
    // Begin printing with red text and print opening '<' of an HTML tag.
    printf( ANSI_TAG_OPEN "<");
    while ( ( ch = getchar() ) != EOF ) {
        putchar( ch );
        if ( ch == '>' ) {
            // Reset output color and escape loop.
            printf( ANSI_RESET );
            break;
        }
    }
    // Error handling.
    if ( ch == EOF ) {
        printf( ANSI_RESET );
        exit( INVALID_TAG );
    }
}

/**
   Outputs the HTML entity read in from standard input with blue text, or exits the program
   with an exit status of 100 if the end of file character is encountered before the closing
   ';' character.
 */
void showEntity()
{
    // Initialize input character variable to compare with EOF character.
    int ch;
    // Begin printing with blue text and print opening '&' of an HTML tag.
    printf( ANSI_ENTITY_OPEN "&" );
    while ( ( ch = getchar() ) != EOF ) {
        putchar( ch );
        if ( ch == ';' ) {
            // Reset output color and escape loop.
            printf( ANSI_RESET );
            break;
        }
    }
    // Error handling.
    if ( ch == EOF ) {
        printf ( ANSI_RESET );
        exit( INVALID_ENTITY );
    }
}

/**
   Starting point of the program. Moves through standard input and outputs the text with the
   appropriate highlighting by calling the showEntity and showTag functions as needed.
   @return Exit status for the program.
 */
int main()
{
    // Initialize input character variable to compare with EOF character.
    int ch;
    while ( ( ch = getchar() ) != EOF ) {
        // Determine if the helper functions are needed based on the character read from input.
        if (ch == '<') {
            showTag();
        } else if (ch == '&') {
            showEntity();
        } else {
            putchar( ch );
        }
    }
    return EXIT_SUCCESS;
}