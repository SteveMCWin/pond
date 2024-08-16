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

    float cohesionIntensity = 5.0f;
    float alignmentIntensity = 5.0f;
    float separationIntensity = 5.0f;

    glm::vec2 calcCohesion(Fish& fish);
    glm::vec2 calcAlignment(Fish& fish);
    glm::vec2 calcSeparation(Fish& fish);

};

#endif // !FISH_HANDLER_H
