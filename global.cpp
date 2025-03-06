#include "global.h"
#include <cstdlib>

// float Global::pi = 3.14159f;
// float Global::screenHalfSize = 80.0f;
// glm::vec2 Global::bottomLeftCorner = glm::vec2(-screenHalfSize, screenHalfSize*aspectRatio);
glm::mat4 Global::projectionMatrix = glm::ortho(-FRUSTUM_HALF_WIDTH, FRUSTUM_HALF_WIDTH, -FRUSTUM_HALF_WIDTH * aspect_ratio, FRUSTUM_HALF_WIDTH * aspect_ratio, -1.0f, 1.0f);

float Global::DegToRad(float degrees){
    // converts degrees to radians
    return degrees*0.01745f;
}

glm::vec2 Global::CalculateNormal(glm::vec2 vec){
    return glm::vec2(glm::normalize(glm::cross(glm::vec3(vec, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f))));
}

float Global::GetRandomFloat(){
    return (float)(rand())/(float)(RAND_MAX);
}

float Global::angleOfVector(glm::vec2 v){
    float angle = std::acos(glm::dot(v, glm::vec2(1.0f, 0.0f)));  // acos returns the angle in radians
    if(v.y < 0.0f){
        angle *= -1.0f;
    }
    return angle;
}

glm::vec2 Global::rotateVector(glm::vec2& vector, float angle){

    angle = Global::DegToRad(angle);

    float new_x = std::cos(angle)*vector.x - std::sin(angle)*vector.y;
    float new_y = std::sin(angle)*vector.x + std::cos(angle)*vector.y;

    return glm::normalize(glm::vec2(new_x, new_y));
}

