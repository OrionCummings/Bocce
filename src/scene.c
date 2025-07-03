#include "scene.h"

bool is_valid_transition(SceneID from, SceneID to) {
    // See the definition of adjacency matrix and "How to use 1D array as a 2D array"
    return (SCENE_TRANSITIONS[from + (to * NUM_SCENES)]);
}