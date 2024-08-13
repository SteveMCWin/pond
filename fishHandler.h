#ifndef FISH_HANDLER_H
#define FISH_HANDLER_H

#include <iostream>
#include <vector>

#include "fish.h"
#include "shader.h"
#include "fishRenderer.h"

class FishHandler{

public:

    void addFish(Fish& fish);
    void addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, float speed = 0.2f);

    std::vector<Fish> allFish;

private:
    Shader circleShader;
    Shader bezierShader;
    Shader outlineShader;

};

#endif // !FISH_HANDLER_H
