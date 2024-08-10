#include "glad.h"
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "fish.h"
#include "shader.h"
#include "bezier_filled.h"

class FishRenderer{

public:
     FishRenderer();
    ~FishRenderer();
    void renderFishBody(const Fish& fish, Shader& circleShader, Shader& outlineShader);
    void renderFishBackFin(const Fish& fish, Shader& finShader);
    void renderFishSideFins(const Fish& fish, glm::vec2 frontScale, glm::vec2 backScale, Shader& finShader);
    void renderFishEyes(const Fish& fish, glm::vec2 scale, Shader& circleShader);
    BezierCurve fin_bezier;

private:

    void renderOvals(glm::vec2 position, glm::vec2 offset, float rotationAngle, glm::vec2 scale, Shader& shader, glm::vec3 color, float r);

    const float aspect_ratio = 9.0f/16.0f;
    const float pi = 3.14159f;

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
