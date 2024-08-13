#include "global.h"

float Global::pi = 3.14159f;
float Global::aspectRatio = 9.0f/16.0f;
float Global::screenHalfSize = 60.0f;
glm::vec2 Global::bottomLeftCorner = glm::vec2(-screenHalfSize, screenHalfSize*aspectRatio);
glm::mat4 Global::projectionMatrix = glm::ortho(-screenHalfSize, screenHalfSize, -screenHalfSize * aspectRatio, screenHalfSize * aspectRatio, -1.0f, 1.0f);

float Global::deg_to_rad(float degrees){
    return degrees*0.01745f;
}
