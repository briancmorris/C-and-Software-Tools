/**
    @file scene.h
    @author Brian Morris (bcmorri3)

    The scene header file contains declarations of all of the functions required to
    manipulate a Scene object and work with Models.
 */
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

/**
    The addModel function loads a Model into the given Scene.

    @param s the Scene to load the Model into.
    @param fname the name of the file to load the Model from.
    @param mname the name of the Model.
 */
void addModel( Scene *s, char const *fname, char const *mname );

/**
    The containsModel function returns true if the given Scene contains a Model with
    the given name.

    @param s a Scene that may contain a Mode with the given name.
    @param mname the name of the Model in question.

    @return true if the given Scene contains the given Model.
 */
bool containsModel( Scene *s, char const *mname );

/**
    The saveScene function saves the the line segments of the models found within the
    given Scene to an output file with the given file name. If the output file can't  be
    opened, an error message is output and no output file is saved.

    @param s the Scene to save.
    @param fname the name of the output file.
 */
void saveScene( Scene *s, char const *fname );

/**
    The removeModel function removes a Model with the give name from the given Scene.

    @param s the Scene to remove a Model from.
    @param mname the name of the Model to remove.
*/
void removeModel( Scene *s, char const *mname );

/**
    The list function lists the information about the Models contained within the given
    Scene.

    @param s the Scene to display information about.
 */
void list( Scene *s );

/**
    The sortModels function sorts all of the Models found within the given Scene
    by Model name, storing them in alphabetical order.

    @param s the Scene that needs to have its Models sorted.
 */
void sortModels( Scene *s );

#endif
