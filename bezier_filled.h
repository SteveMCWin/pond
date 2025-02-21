#ifndef BEZIER_FILLED_H
#define BEZIER_FILLED_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shader.h"

class BezierCurve{

public:
     BezierCurve();
    ~BezierCurve();

    void DrawBezierFilled(int numOfPoints, glm::vec2 worldPos, glm::vec2 p1, glm::vec2 p2, glm::vec2 controlPoint, Shader& shader, glm::vec3 Color);
    void DrawCrescentBezierFilled(int numOfPoints, glm::vec2 worldPos, glm::vec2 p1, glm::vec2 p2, glm::vec2 controlPoint, glm::vec2 controlPoint2, Shader& shader, glm::vec3 color);

private:
    unsigned int VBO;
    unsigned int VAO;

    unsigned int crescentVBO;
    unsigned int crescentVAO;

    float aspect_ratio = 9.0f/16.0f;

    float triangle_vertices[6];
    std::vector<float> crescent_vertices;
    std::vector<float> crescent_line_vertices;

    int max_crescent_vertices = 20;

};

#endif
