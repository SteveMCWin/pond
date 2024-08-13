#include "global.h"

float Global::aspectRatio = 9.0f/16.0f;
float Global::screenHalfSize = 60.0f;
glm::mat4 Global::projectionMatrix = glm::ortho(-screenHalfSize, screenHalfSize, -screenHalfSize * aspectRatio, screenHalfSize * aspectRatio, -1.0f, 1.0f);
