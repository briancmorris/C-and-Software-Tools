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
    int ch;

    /** The bit currently being examined by the function. */
    unsigned char bit;

    // The first bit found.
    unsigned char firstBit;

    // The binary code that has been parsed from the character.
    int code = 0x0000;

    // Counter to keep track of the number of consecutive zeros found in ch.
    int zeroCount = 0;

    // Counter to keep track of the number of ones found in ch.
    int oneCount = 0;

    // The number of bits in the ASCII code.
    int numBits = 0;

    // While there's content in the buffer.
    while ( buffer->bcount > 0 ) {
        // Decrease bcount.
        buffer->bcount--;
        // Get the high order bit from bits.
        bit = (buffer->bits >> buffer->bcount) & 0x01;
        // Shift the code to make space for an additional bit.
        code = code << 1;
        // Append the bit to code.
        code |= bit;

        // Get the first bit.
        if ( numBits == 0 ) {
            firstBit = bit;
        }

        // If the bit is a 1, else 0.
        if ( bit ) {
            oneCount++;
            zeroCount = 0;
        } else {
            zeroCount++;
        }

        // Clear the bit inside the buffer.
        buffer->bits &= ~( 1 << buffer->bcount );
        if ( zeroCount == 2 ){
            return code;
        }
        // Increase number of bits.
        numBits++;
    }

    // While we need an additional character from the file.
    while ( ( ch = fgetc( fp ) ) != EOF ) {
        for ( int i = 0; i < BITS_PER_BYTE; i++ ){
            // Get the i-th high order bit.
            bit = ( ch >> ( BITS_PER_BYTE - i - 1 ) ) & 0x01;
            // Shift the code to make space for an additional bit.
            code = code << 1;
            // Append the bit to code.
            code |= bit;

            // Find the first bit.
            if ( numBits == 0 ) {
                firstBit = bit;
            }

            // If the bit is a 1, else 0.
            if ( bit ) {
                oneCount++;
                zeroCount = 0;
            } else {
                zeroCount++;
            }

            // If the end of the code has been reached.
            if ( zeroCount == 2 ){
                // Number of remaining bits to add to the buffer.
                int remainingBits = BITS_PER_BYTE - i;
                // Put the remaining bits from ch into the buffer.
                for ( int j = 0; j < remainingBits; j++ ) {
                    // Shift bits to make space for an additional bit.
                    buffer->bits = buffer->bits << 1;
                    // Create the mask for the j-th high order bit.
                    int mask = ( ch >> (remainingBits - j - 1 ) ) & 0x01;
                    // Append the bit.
                    buffer->bits |= mask;
                    // Increase bcount.
                    buffer->bcount++;
                }
                // Return the code.
                return code;
            }
            // Increase the number of bits used in this code.
            numBits++;
        }
    }

    // If the first bit was 1 and two 0's weren't found, else return -1.
    if ( firstBit && zeroCount < 2 ) {
        return -2;
    } else {
        return -1;
    }
}
