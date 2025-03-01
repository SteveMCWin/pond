#ifndef GLOBAL_H
#define GLOBAL_H

#include <glad/glad.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Global{
public:
    static constexpr float fishSightRange = 25.0;
    static const int numberOfFish = 15;
    static const int numberOfJoints = 12;
    static float aspectRatio;
    static float screenHalfSize;
    static float pi;
    static glm::vec2 bottomLeftCorner;
    static glm::mat4 projectionMatrix;

    static float DegToRad(float degrees);   // converts degrees to radians
    static glm::vec2 CalculateNormal(glm::vec2 vec);    // returns a normal vector pointing to the right if the input vec is pointing up
    static float GetRandomFloat();  // returns float ranging from 0.0 to 1.0
    static float angleOfVector(glm::vec2 v);   // returns the angle in radians between the input angle and the <1.0, 0.0> vector (return value ranges from -pi to pi)
    static glm::vec2 rotateVector(glm::vec2& vector, float angle);   // rotates a vector a specified amount of degrees
};

#endif // !GLOBAL_H
