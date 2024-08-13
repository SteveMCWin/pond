#ifndef GLOBAL_H
#define GLOBAL_H

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Global{
public:
    static float aspectRatio;
    static glm::mat4 projectionMatrix;
    static float screenHalfSize;
};

#endif // !GLOBAL_H
