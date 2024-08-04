#include "glad.h"
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "fish.h"
#include "shader.h"

class FishRenderer{

public:
     FishRenderer();
    ~FishRenderer();
    void renderFish(const Fish& fish, Shader& circleShader, Shader& outlineShader);

private:

    const float aspect_ratio = 9.0f/16.0f;

    float vertices[12] = {
        -1.0, -1.0,
         1.0, -1.0,
        -1.0,  1.0,

         1.0, -1.0,
         1.0,  1.0,
        -1.0,  1.0
    };

    unsigned int circleVBO, circleVAO;
    unsigned int outlineVBO, outlineVAO;

};
