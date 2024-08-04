#include "fishRenderer.h"
#include <GLFW/glfw3.h>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

FishRenderer::FishRenderer(){
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

FishRenderer::~FishRenderer(){
    glDeleteBuffers(1, &this->VBO);
    glDeleteVertexArrays(1, &this->VAO);
}

void FishRenderer::renderFish(const Fish& fish, Shader& shader){

    glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f * this->aspect_ratio, 20.0f * this->aspect_ratio, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    glBindVertexArray(this->VAO);

    for(Joint joint : fish.joints){
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(joint.Center, 0.0f));
        model = glm::scale(model, glm::vec3(joint.circleRadius, joint.circleRadius, 1.0f));

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);
        shader.setFloat("r", joint.circleRadius);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);

}
