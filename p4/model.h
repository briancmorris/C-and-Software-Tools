#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>

/** Maximum length of name and filename strings. */
#define NAME_LIMIT 20

/** The number of coordinates each point of a line segment contains. */
#define NUM_COORDS 2

/** Representation for a model, a collection of line segments. */
typedef struct {
    /** Name of the model. */
    char name[ NAME_LIMIT + 1 ];

    /** File name it was loaded from. */
    char fname[ NAME_LIMIT + 1 ];

    /** Number of points in the model. It has half this many line segments. */
    int pCount;

    /**
        List of points in the model, twice as long as the number
        of segments, since each segment has two points
     */
    double (*pList)[ NUM_COORDS ];
} Model;

/**
    This function reads a model from a file with the given name, returning a pointer to a
    dynamically allocated instance of model. If the input file can't be opened or the Model
    isn't in the right format, an error message is printed and NULL is returned.

    @param fname the name of the input file.

    @return A pointer to a dynamically allocated instance of model, or NULL if there is an error.
 */
Model *loadModel( char const *fname );

/**
    This function frees the dynamically allocated memory used to store the given Model including
    the Model itself and the list of points.

    @param m the Model to be freed.
 */
void freeModel( Model *m );

/**
    This function applies a geometric transformation to every line segment in the given Model.
    This is accomplished by using the given function f, on the line segments. The parameters a
    and b are the values used to transform the Model. One or both of a and b will be used to
    apply the transformation, depending on the type of transformation.

    @param m the Model to apply the geometric transformation to.
    @param f the transformation function to be applied to the Model.
    @param a the first required value for transformation.
    @param b the second required value for transformation.
 */
void applyToModel( Model *m, void (*f)( double pt[ NUM_COORDS ], double a, double b ), double a,
                   double b );

#endif
