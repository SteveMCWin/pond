#ifndef FISH_HANDLER_H
#define FISH_HANDLER_H

#include <iostream>
#include <vector>
#include <glm/detail/func_geometric.hpp>
#include <iostream>

#include "global.h"
#include "fish.h"
#include "shader.h"
#include "compute_shader.h"
#include "fishRenderer.h"

typedef struct {
    int* fish_indexes;
    int  num_of_boxed_fish;
} box_struct;

typedef struct {
    glm::vec2 move_dir;
    glm::vec2 cetner;
    int result;
    int padding;
} hit_check_struct;

class FishHandler{

public:

    Fish* allFish;

    unsigned int number_of_fish;

    float cohesionIntensity = 0.5f;
    float alignmentIntensity = 180.0f;
    float separationIntensity = 180.0f;
    float edgeEvasionIntensity = 80.0f;

    FishHandler();
    ~FishHandler();

    void createFish();

    void boxTheFish();
    void resetBoxSizes();
    void calcFishHitChecks();
    void handleFishMovement(float delta_time);    // responsible for boid-like behaviour of fish

private:

    box_struct** boxes;

};

#endif // !FISH_HANDLER_H
