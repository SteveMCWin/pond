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

    FishHandler();  // the class is (at least at the moment) used only for storing and accessing all the fish created

    void addFish(Fish& fish);
    void addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int id, int numOfHitChecks, float speed = 15.0f);
    glm::vec2 calcFishMoveDir(Fish& fish);

private:

    float cohesionIntensity = 0.1f;
    float alignmentIntensity = 0.5f;
    float separationIntensity = 0.1f;
    float edgeEvasionIntensity = 2.0f;
};

#endif // !FISH_HANDLER_H
