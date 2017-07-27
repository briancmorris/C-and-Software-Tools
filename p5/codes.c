/**
    @file codes.c
    @author Brian Morris (bcmorri3)

    Implementation of the codes.h component, with functions supporting
    the translation of ASCII codes to binary code and vice versa.
*/

#include "codes.h"

int symToCode( unsigned char ch )
{
    unsigned char keys[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                             'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ',
                             '\n' };

    int values[] = { 0x2C, 0x354, 0x6B4, 0xD4, 0x4, 0x2B4, 0x1B4, 0x154, 0x14, 0xB6C, 0x1AC, 0x2D4,
                     0x6C, 0x34, 0x36C, 0x5B4, 0xDAC, 0xB4, 0x54, 0xC, 0xAC, 0x2AC, 0x16C, 0x6AC,
                     0xD6C, 0x6D4, 0x5AC, 0x56C };

    for ( int i = 0; i < NUM_CODES; i++ ) {
        if ( keys[ i ] == ch ) {
            return values[ i ];
        }
    }

    return INVALID_CODE;
}

int bitsInCode( unsigned char ch )
{
    unsigned char keys[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                             'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ',
                             '\n' };

    int values[] = { SIX_BITS, TEN_BITS, ELEVEN_BITS, EIGHT_BITS, THREE_BITS, TEN_BITS, NINE_BITS,
                     NINE_BITS, FIVE_BITS, TWELVE_BITS, NINE_BITS, TEN_BITS, SEVEN_BITS, SIX_BITS,
                     TEN_BITS, ELEVEN_BITS, TWELVE_BITS, EIGHT_BITS, SEVEN_BITS, FOUR_BITS,
                     EIGHT_BITS, TEN_BITS, NINE_BITS, ELEVEN_BITS, TWELVE_BITS, ELEVEN_BITS,
                     ELEVEN_BITS, ELEVEN_BITS };

    for ( int i = 0; i < NUM_CODES; i++ ) {
        if ( keys[ i ] == ch ) {
            return values[ i ];
        }
    }

    return INVALID_CODE;
}

int codeToSym( int code )
{
    int keys[] = { 0x2C, 0x354, 0x6B4, 0xD4, 0x4, 0x2B4, 0x1B4, 0x154, 0x14, 0xB6C, 0x1AC, 0x2D4,
                   0x6C, 0x34, 0x36C, 0x5B4, 0xDAC, 0xB4, 0x54, 0xC, 0xAC, 0x2AC, 0x16C, 0x6AC,
                   0xD6C, 0x6D4, 0x5AC, 0x56C };

    int values[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                     'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                     ' ', '\n' };

    for ( int i = 0; i < NUM_CODES; i++ ) {
        if ( keys[ i ] == code ) {
            return values[ i ];
        }
    }

    return INVALID_CODE;
}
