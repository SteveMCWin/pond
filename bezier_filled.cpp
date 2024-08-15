#include "bezier_filled.h"
#include "global.h"
#include <glm/detail/func_geometric.hpp>
#include <iostream>

BezierCurve::BezierCurve(){
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindVertexArray(this->VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->triangle_vertices), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glGenBuffers(1, &this->crescentVBO);
    glGenVertexArrays(1, &this->crescentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->crescentVBO);
    glBindVertexArray(this->crescentVAO);

    glBufferData(GL_ARRAY_BUFFER, (4 * max_crescent_vertices - 4) * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BezierCurve::~BezierCurve(){
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->crescentVBO);
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteVertexArrays(1, &this->crescentVAO);
}

void BezierCurve::DrawBezierFilled(int numOfPoints, glm::vec2 worldPos, glm::vec2 p1, glm::vec2 p2, glm::vec2 controlPoint, Shader& shader, glm::vec3 color){

    this->triangle_vertices[0] = p2.x;
    this->triangle_vertices[1] = p2.y;
    this->triangle_vertices[2] = p1.x;
    this->triangle_vertices[3] = p1.y;

    glm::vec2 first_moveable  = controlPoint - p1;
    glm::vec2 second_moveable = p2 - controlPoint;

    glBindVertexArray(this->VAO);
    
    glm::mat4 projection = Global::projectionMatrix;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(worldPos, 0.0));

    for(int i = 0; i < numOfPoints; i++){

        glm::vec2 move_from_start = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * first_moveable;
        glm::vec2 move_to_end = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * second_moveable;

        glm::vec2 one_to_another = (controlPoint + move_to_end - (p1 + move_from_start));

        glm::vec2 final_vector = p1 + move_from_start + (1.0f/(numOfPoints-1)) * static_cast<float>(i) * one_to_another;

        this->triangle_vertices[4] = final_vector.x;
        this->triangle_vertices[5] = final_vector.y;

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), this->triangle_vertices);

        shader.use();
        shader.setVec3("Color", color.x, color.y, color.z);
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        this->triangle_vertices[2] = final_vector.x;
        this->triangle_vertices[3] = final_vector.y;

    }

    glBindVertexArray(0);

}


void BezierCurve::DrawCrescentBezierFilled(int numOfPoints, glm::vec2 worldPos, glm::vec2 p1, glm::vec2 p2, glm::vec2 controlPoint, glm::vec2 controlPoint2,
                                           Shader& shader, glm::vec3 color){

    numOfPoints = std::min(numOfPoints, max_crescent_vertices);

    this->crescent_vertices.resize(0);

    this->crescent_vertices.push_back(p1.x);
    this->crescent_vertices.push_back(p1.y);

    glm::vec2 first_moveable  = controlPoint - p1;
    glm::vec2 second_moveable = p2 - controlPoint;

    glm::mat4 projection = Global::projectionMatrix;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(worldPos, 0.0));

    glm::vec2 first_moveable2  = controlPoint2 - p1;
    glm::vec2 second_moveable2 = p2 - controlPoint2;

    for(int i = 1; i < numOfPoints-1; i++){

        glm::vec2 move_from_start = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * first_moveable;
        glm::vec2 move_to_end = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * second_moveable;

        glm::vec2 one_to_another = (controlPoint + move_to_end - (p1 + move_from_start));

        glm::vec2 final_vector = p1 + move_from_start + (1.0f/(numOfPoints-1)) * static_cast<float>(i) * one_to_another;

        this->crescent_vertices.push_back(final_vector.x);
        this->crescent_vertices.push_back(final_vector.y);

        move_from_start = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * first_moveable2;
        move_to_end = (1.0f/(numOfPoints-1)) * static_cast<float>(i) * second_moveable2;

        one_to_another = (controlPoint2 + move_to_end - (p1 + move_from_start));

        final_vector = p1 + move_from_start + (1.0f/(numOfPoints-1)) * static_cast<float>(i) * one_to_another;

        this->crescent_vertices.push_back(final_vector.x);
        this->crescent_vertices.push_back(final_vector.y);
    }

    this->crescent_vertices.push_back(p2.x);
    this->crescent_vertices.push_back(p2.y);


    glBindBuffer(GL_ARRAY_BUFFER, this->crescentVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->crescent_vertices.size() * sizeof(float), this->crescent_vertices.data());

    shader.use();
    shader.setVec3("Color", color.x, color.y, color.z);
    shader.setMat4("projection", projection);
    shader.setMat4("model", model);

    glBindVertexArray(this->crescentVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->crescent_vertices.size()/2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
