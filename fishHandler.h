#ifndef FISH_HANDLER_H
#define FISH_HANDLER_H

#include <iostream>
#include <vector>
#include <glm/detail/func_geometric.hpp>
#include <iostream>

#include "global.h"
#include "fish.h"
#include "shader.h"
#include "fishRenderer.h"

typedef struct {
    int* fish_indexes;
    int  num_of_boxed_fish;
} box_struct;

class FishHandler{

public:

    Fish* allFish;

    unsigned int number_of_fish;

    float cohesionIntensity;
    float alignmentIntensity;
    float separationIntensity;
    float edgeEvasionIntensity;

    FishHandler();
    ~FishHandler();

    void createFish();

    void boxTheFish();
    void resetBoxSizes();
    void calcFishHitChecks();
    void handleFishMovement(float delta_time);    // responsible for boid-like behaviour of fish

    void update_num_of_fish();
    void update_cohesion_intensity();
    void update_separation_intensity();
    void update_alignment_intensity();
    void update_edge_evasion_intensity();

private:

    box_struct** boxes;

};

#endif // !FISH_HANDLER_H
