#ifndef FISH_HANDLER_H
#define FISH_HANDLER_H

#include <iostream>
#include <vector>

#include "fish.h"
#include "shader.h"
#include "fishRenderer.h"

class FishHandler{

public:
    FishHandler();

    void addFish(Fish& fish);
    void addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int numOfHitChecks, float speed = 20.0f);

    std::vector<Fish> allFish;

private:
    // Shader circleShader;
    // Shader bezierShader;
    // Shader outlineShader;

};

#endif // !FISH_HANDLER_H
