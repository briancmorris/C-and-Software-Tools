# Default options.
CC = gcc
CFLAGS = -g -Wall -std=c99
LDLIBS = -lm

# Executable
all: drawing

drawing: model.o scene.o
drawing.o: scene.h model.h

scene.o: scene.h model.h
model.o: model.h

# Cleanup files.
clean:
	rm -f drawing.o drawing
	rm -f scene.o scene
	rm -f model.o model
	rm -f output.txt