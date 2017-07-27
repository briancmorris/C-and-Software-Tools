/**
    @file codes.h
    @author Brian Morris (bcmorri3)

    Header file for the codes.c component, with functions supporting
    the translation of ASCII codes to binary code and vice versa.
*/

/** The value that is returned if the given ASCII code or binary code is invalid. */
#define INVALID_CODE -1

/**
    The number of binary codes that correspond the number of ASCII codes and binary codes handled
    by this program.
*/
#define NUM_CODES 28

/** The number of bits of a binary code that uses 3 bits. */
#define THREE_BITS 3

/** The number of bits of a binary code that uses 4 bits. */
#define FOUR_BITS 4

/** The number of bits of a binary code that uses 5 bits. */
#define FIVE_BITS 5

/** The number of bits of a binary code that uses 6 bits. */
#define SIX_BITS 6

/** The number of bits of a binary code that uses 7 bits. */
#define SEVEN_BITS 7

/** The number of bits of a binary code that uses 8 bits. */
#define EIGHT_BITS 8

/** The number of bits of a binary code that uses 9 bits. */
#define NINE_BITS 9

/** The number of bits of a binary code that uses 10 bits. */
#define TEN_BITS 10

/** The number of bits of a binary code that uses 11 bits. */
#define ELEVEN_BITS 11

/** The number of bits of a binary code that uses 12 bits. */
#define TWELVE_BITS 12

/**
    Given the ASCII code for a character, this function returns the binary code used to represent
    it. If there's no binary code to represent the given character, this function returns -1.

    @param ch The ASCII code to change to binary code.

    @return The binary code used to represent the given ASCII code, or -1 if it's invalid.
 */
int symToCode( unsigned char ch );

/**
    Given the ASCII code for a character, this function returns the the number of bits in the
    binary code used to represent it. If there's no binary code to represent the given character,
    this function returns -1.

    @param ch The ASCII code to retrieve the number of bits of.

    @return The number of bits that represent the given ASCII code, or -1 if invalid.
 */
int bitsInCode( unsigned char ch );

/**
    Given binary code, this function returns the ASCII code it represents. If the
    binary code does not represent a valid character, -1 is returned.

    @param code The binary code to change to ASCII code.

    @return The ASCII code the binary code represents, or -1 if invalid.
 */
int codeToSym( int code );
