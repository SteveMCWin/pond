#ifndef GLOBAL_H
#define GLOBAL_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Global{
public:
    static float aspectRatio;
    static float screenHalfSize;
    static float pi;
    static glm::vec2 bottomLeftCorner;
    static glm::mat4 projectionMatrix;

    static float DegToRad(float degrees);   // converts degrees to radians
    static glm::vec2 CalculateNormal(glm::vec2 vec);    // returns a normal vector pointing to the right if the input vec is pointing up
    static float GetRandomFloat();  // returns float ranging from 0.0 to 1.0
    static float angleBetweenVectors(glm::vec2 v1, glm::vec2 v2);   // returns the angle in radians between two vectors (return value ranges from -pi to pi)
    static glm::vec2 rotateVector(glm::vec2& vector, float angle);   // rotates a vector a specified amount of degrees
};

#endif // !GLOBAL_H
