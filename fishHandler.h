#ifndef FISH_HANDLER_H
#define FISH_HANDLER_H

#include <iostream>
#include <vector>

#include "fish.h"
#include "shader.h"
#include "fishRenderer.h"

class FishHandler{

public:

    std::vector<Fish> allFish;

    FishHandler();

    void addFish(Fish& fish);
    void addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int id, int numOfHitChecks, float speed = 15.0f);

    glm::vec2 calcFishMoveDir(Fish& fish, float delta_time);

private:

    float cohesionIntensity = 0.3f;
    float alignmentIntensity = 13.0f;
    float separationIntensity = 130.0f;
    float edgeEvasionIntensity = 50.0f;
};

#endif // !FISH_HANDLER_H
