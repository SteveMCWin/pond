#ifndef FISH_HANDLER_H
#define FISH_HANDLER_H

#include <iostream>
#include <vector>

#include "fish.h"
#include "shader.h"
#include "compute_shader.h"
#include "fishRenderer.h"

typedef struct {
    glm::vec2 move_dir;
    glm::vec2 cetner;
    int results[13];    // HARD CODED FOR NOW, WILL TRY TO MAKE THE COMPUTE SHADER SUM UP THE RESULTS
} hit_check_struct;

class FishHandler{

public:

    std::vector<Fish> allFish;

    FishHandler();

    void addFish(Fish& fish);
    void addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int id, float speed = 15.0f);

    void calcFishHitChecks();
    glm::vec2 calcFishMoveDir(Fish& fish, float delta_time);    // responsible for boid-like behaviour of fish

private:

    unsigned int ssbo;
    float cohesionIntensity = 0.5f;
    float alignmentIntensity = 180.0f;
    float separationIntensity = 180.0f;
    float edgeEvasionIntensity = 80.0f;
};

#endif // !FISH_HANDLER_H
