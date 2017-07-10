#ifndef _SCENE_H_
#define _SCENE_H_

#include "model.h"
#include <stdbool.h>

/** Value used to initialize and resize an array of model pointers. */
#define RESIZE 2

/** Representation for a whole scene, a collection of models. */
typedef struct {
    /** Number of models in the scene. */
    int mCount;

    /** Capacity of the model list. */
    int mCap;

    /** List of pointers to models. */
    Model **mList;
} Scene;

/**
    This function dynamically allocates an instance of Scene and initializes its fields.

    @return A dynamically allocated instance of Scene.
 */
Scene *makeScene();

/**
    This function frees all of the dynamically allocated memory used by a scene, including the
    Scene object itself, its array of Model pointers, and the Model instances themselves.

    @param s the Scene to be freed.
 */
void freeScene( Scene *s );

/**
    This function finds the Model with the given name and applies the provided transformation
    function to the Model.

    @param s the Scene containing the Model to be transformed.
    @param name the name of the Model to be transformed.
    @param f the transformation function to be applied to the Model.
    @param a the first required value for transformation.
    @param b the second required value for transformation.

    @return True if the transformation was applied, false if the given name is not used by
            an existing Model.
 */
bool applyToScene(Scene *s, char const *name, void (*f)(double pt[NUM_COORDS], double a, double b),
                  double a, double b);

void addModel( Scene *s, char const *fname, char const *name );

bool containsModel( Scene *s, char const *name );

void saveScene( Scene *s, char const *fname );

void removeModel( Scene *s, char const *mname );

void list( Scene *s );

void sortModels( Scene *s );

#endif
