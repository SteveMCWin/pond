#include "bezier_filled.h"

BezierCurve::BezierCurve(){
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindVertexArray(this->VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->triangle_vertices), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BezierCurve::~BezierCurve(){
    glDeleteBuffers(1, &this->VBO);
    glDeleteVertexArrays(1, &this->VAO);
}

void BezierCurve::DrawBezierFilled(int numOfPoints, glm::vec2 worldPos, glm::vec2 p1, glm::vec2 p2, glm::vec2 attractor, Shader& shader){

    this->triangle_vertices[0] = p2.x;
    this->triangle_vertices[1] = p2.y;
    this->triangle_vertices[2] = p1.x;
    this->triangle_vertices[3] = p1.y;

    glm::vec2 first_moveable  = attractor - p1;
    glm::vec2 second_moveable = p2 - attractor;

    glBindVertexArray(this->VAO);

    for(int i = 0; i < numOfPoints; i++){

        glm::vec2 move_from_start = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * first_moveable;
        glm::vec2 move_to_end = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * second_moveable;

        glm::vec2 one_to_another = (attractor + move_to_end - (p1 + move_from_start));

        glm::vec2 final_vector = p1 + move_from_start + (1.0f/(numOfPoints-1)) * static_cast<float>(i) * one_to_another;

        this->triangle_vertices[4] = final_vector.x;
        this->triangle_vertices[5] = final_vector.y;

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), this->triangle_vertices);

        shader.use();
        glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f * this->aspect_ratio, 20.0f * this->aspect_ratio, -1.0f, 1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(worldPos, 0.0));

        shader.setMat4("projection", projection);
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        this->triangle_vertices[2] = final_vector.x;
        this->triangle_vertices[3] = final_vector.y;

    }

    glBindVertexArray(0);

}
