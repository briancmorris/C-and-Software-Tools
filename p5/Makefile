# Default options.
CC = gcc
CFLAGS = -g -Wall -std=c99

# Executables
all: encrypt decrypt

encrypt: codes.o bits.o
encrypt.o: codes.h bits.h

decrypt: codes.o bits.o
decrypt.o: codes.h bits.h

codes.o: codes.h
bits.o: bits.h

# Cleanup files.
clean:
	rm -f encrypt.o encrypt
	rm -f decrypt.o decrypt
	rm -f codes.o codes
	rm -f bits.o bits
	rm -f output.txt