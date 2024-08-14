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

    static float DegToRad(float degrees);
    static glm::vec2 CalculateNormal(glm::vec2 vec);
    static float GetRandomFloat();
};

#endif // !GLOBAL_H
