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
    // The number of leading 0's in the given binary code.
    int leadingZeros = BITS_PER_INT - nbits;

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
        unsigned char mask = ( code >> ( leadingZeros - 7 + i ) ) & 0x01;

        // Set bits to the i-th high-order value in code.
        buffer->bits = buffer->bits | mask;

        // Increase bcount.
        buffer->bcount++;
    }
}

void flushBits( BitBuffer *buffer, FILE *fp )
{
    // Determine the number of ending zeros that must be added to the buffer.
    int endingZeros = BITS_PER_BYTE - buffer->bcount;

    // Print the contents of the buffer.
    fprintf( fp, "%c", ( buffer->bits << endingZeros ) );

    // Reset the contents of buffer.
    buffer->bits = 0x00;
    buffer->bcount = 0;
}

int readBits (BitBuffer *buffer, FILE *fp )
{
    return 0;
}
