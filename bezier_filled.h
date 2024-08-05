#ifndef BEZIER_FILLED_H
#define BEZIER_FILLED_H
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class BezierCurve{

public:
     BezierCurve();
    ~BezierCurve();

    void DrawBezierFilled(int numOfPoints, glm::vec2 worldPos, glm::vec2 p1, glm::vec2 p2, glm::vec2 attractor, Shader& shader);

private:
    unsigned int VBO;
    unsigned int VAO;

    float aspect_ratio = 9.0f/16.0f;

    float triangle_vertices[6];

};

#endif
