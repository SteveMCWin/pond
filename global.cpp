#include "global.h"
#include <cstdlib>

float Global::pi = 3.14159f;
float Global::aspectRatio = 9.0f/16.0f;
float Global::screenHalfSize = 60.0f;
glm::vec2 Global::bottomLeftCorner = glm::vec2(-screenHalfSize, screenHalfSize*aspectRatio);
glm::mat4 Global::projectionMatrix = glm::ortho(-screenHalfSize, screenHalfSize, -screenHalfSize * aspectRatio, screenHalfSize * aspectRatio, -1.0f, 1.0f);

float Global::DegToRad(float degrees){
    // converts degrees to radians
    return degrees*0.01745f;
}

glm::vec2 Global::CalculateNormal(glm::vec2 vec){
    // returns a normal vector pointing to the right if the input vec is pointing up
    return glm::vec2(glm::normalize(glm::cross(glm::vec3(vec, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f))));
}

float Global::GetRandomFloat(){
    return (float)(rand())/(float)(RAND_MAX);
}
