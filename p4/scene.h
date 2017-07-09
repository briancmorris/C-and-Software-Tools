#ifndef _SCENE_H_
#define _SCENE_H_

#include "model.h"
#include <stdbool.h>

/** Representation for a whole scene, a collection of models. */
typedef struct {
  /** Number of models in the scene. */
  int mCount;
  
  /** Capacity of the model list. */
  int mCap;

  /** List of pointers to models. */
  Model **mList;
} Scene;

#endif
