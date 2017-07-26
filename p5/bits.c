/**
    @file bits.c
    @author Brian Morris (bcmorri3)

    Implementation for the bits.h component, with functions supporting
    reading and writing a file a byte at a time and buffering bits for
    codes that extend over byte boundaries.
*/

#include <stdio.h>
#include "bits.h"

void writeBits( int code, int nbits, BitBuffer *buffer, FILE *fp )
{
    // For the number of bits in the binary code.
    for ( int i = 0; i < nbits; i++ ) {
        // If the buffer is full, print and reset its values.
        if ( buffer->bcount == BITS_PER_BYTE ) {
            fprintf( fp, "%c", buffer->bits );
            buffer->bits = 0x00;
            buffer->bcount = 0;
        } else {
            // Shift the current contents of the buffer by 1 to make room for a new value.
            buffer->bits = ( buffer->bits << 1 );
        }

        // Create a mask that contains only the i-th high order bit of code.
        unsigned char mask = ( code >> ( nbits - i - 1 ) ) & 0x01;

        // Set bits to the i-th high-order value in code.
        buffer->bits |= mask;

        // Increase bcount.
        buffer->bcount++;
    }
}

void flushBits( BitBuffer *buffer, FILE *fp )
{
    // Determine the number of ending zeros that must be added to the buffer and shift.
    int endingZeros = BITS_PER_BYTE - buffer->bcount;
    buffer->bits = buffer->bits << endingZeros; 

    // Print the contents of the buffer.
    fprintf( fp, "%c", buffer->bits );

    // Reset the contents of buffer.
    buffer->bits = 0x00;
    buffer->bcount = 0;
}

int readBits ( BitBuffer *buffer, FILE *fp )
{
    // Char to read the bits of, if needed.
    unsigned char ch;

    /** The bit currently being examined by the function. */
    unsigned char bit;

    // The first bit found.
    unsigned char firstBit;

    // The binary code that has been parsed from the character.
    int code = 0x0000;

    // The number of characters currently used by this function, at most 2.
    int charCount = 0;

    // Counter to keep track of the number of consecutive zeros found in ch.
    int zeroCount = 0;

    // Counter to keep track of the number of ones found in ch.
    int oneCount = 0;

    // Int that is 1 if EOF has been found.
    int eofFound = 0;

    // The number of bits in the ASCII code.
    int numBits = 0;

    // The current shift amount needed to move bits.
    int i = 0;

    while ( zeroCount < 2 ) {
        if ( numBits < buffer->bcount ) {
            // Get the i-th high order bit from bits.
            bit = buffer->bits >> ( BITS_PER_BYTE - i - 1 ) & 0x01;
            // Get the first bit.
            if ( numBits == 0 ) {
                firstBit =  bit;
            }
            // Shift the code to make space for an additional bit.
            code = code << 1;
            // Append the bit to code.
            code |= bit;
        } else {
            // Get the first and second required character if needed.
            if ( charCount == 0 && numBits - buffer->bcount == 0 ) {
                i = 0;
                ch = fgetc( fp );
                charCount++;
            } else if ( charCount == 1 && numBits - BITS_PER_BYTE - buffer->bcount == 0 ) {
                i = 0;
                ch = fgetc( fp );
                charCount++;
            }

            // If ch is EOF.
            if ( ch == EOF ) {
                eofFound++;
                // If no bits or only 0's have been read, return -1.
                if ( numBits == 0 || oneCount == 0 ) {
                    return -1;
                } else if( firstBit == 1 && zeroCount < 2 ) {
                    return -2;
                }
            }

            // Get the i-th high order bit from ch.
            bit = ch >> ( BITS_PER_BYTE - i - 1 ) & 0x01;
            // Shift code to ensure there is room for 1 more bit.
            code = code << 1;
            // Append the current bit to the code.
            code |= bit;
            // Get first bit if needed.
            if ( numBits == 0 ) {
                firstBit = bit;
            }
        }

        // If the bit is a one.
        if ( bit == 1 ) {
            oneCount++;
            // If the first bit read was 0 and EOF has not been found, return -2.
            if( firstBit == 0 && eofFound == 0 ) {
                return -2;
            } else if ( zeroCount > 0 ) { // Reset consecutive 0 counter.
                zeroCount = 0;
            }
        } else {
            zeroCount++;
        }
        numBits++;
        i++;
    }

    if ( ( BITS_PER_BYTE  - i ) > 0 && charCount > 0 ) {
        // The number of bits that need to be added to the buffer from ch.
        int addToBuffer = BITS_PER_BYTE - i;
        // Mask for appending values to buffer.
        unsigned char mask;
        // Assign the new value of bcount.
        buffer->bcount = addToBuffer;

        // Get the remaining bits in ch.
        for ( int j = 0; j < addToBuffer; j++ ) {
            // Shift bits to append a bit.
            buffer->bits = buffer->bits << 1;
            // Create the mask for the j-th high order bit.
            mask = ch >> ( BITS_PER_BYTE - i - 1 ) & 0x01;
            // Append the bit.
            buffer->bits |= mask;
            i++;
        }
    }

    return code;
}
