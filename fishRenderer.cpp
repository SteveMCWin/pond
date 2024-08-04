#include "fishRenderer.h"
#include <GLFW/glfw3.h>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

FishRenderer::FishRenderer(){
    glGenBuffers(1, &this->circleVBO);
    glGenVertexArrays(1, &this->circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->circleVBO);
    glBindVertexArray(this->circleVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glGenBuffers(1, &this->outlineVBO);
    glGenVertexArrays(1, &this->outlineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->outlineVBO);
    glBindVertexArray(this->outlineVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

FishRenderer::~FishRenderer(){
    glDeleteBuffers(1, &this->circleVBO);
    glDeleteVertexArrays(1, &this->circleVAO);
}

void FishRenderer::renderFish(const Fish& fish, Shader& circleShader, Shader& outlineShader){

    glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f * this->aspect_ratio, 20.0f * this->aspect_ratio, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    // render the circles

    glBindVertexArray(this->circleVAO);

    for(Joint joint : fish.joints){
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(joint.Center, 0.0f));
        model = glm::scale(model, glm::vec3(joint.circleRadius, joint.circleRadius, 1.0f));

        circleShader.use();
        circleShader.setMat4("projection", projection);
        circleShader.setMat4("model", model);
        circleShader.setFloat("r", joint.circleRadius);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // render the outline

    glBindVertexArray(this->outlineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->outlineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(fish.outline_vertices), &fish.outline_vertices);

    outlineShader.use();
    outlineShader.setMat4("projection", projection);

    for(int i = 0; i < 5; i++){

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(fish.joints[i].Center, 0.0f));

        outlineShader.setMat4("model", model);

        glDrawArrays(GL_LINES, i*2, i*2+1);
    }

    // glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);

    glBindVertexArray(0);

}
