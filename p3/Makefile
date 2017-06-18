# Default options.
CC = gcc
CFLAGS = -g -Wall -std=c99

# Executables
all: jumble puzzle

# Jumble
jumble: jumble.o

# Puzzle
puzzle: puzzle.o grid.o

puzzle.o: grid.h
grid.o: grid.h

# Cleanup files.
clean:
	rm -f jumble.o jumble
	rm -f puzzle.o puzzle
	rm -f grid.o grid
	rm -f output.txt