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

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 48, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

FishRenderer::~FishRenderer(){
    glDeleteBuffers(1, &this->circleVBO);
    glDeleteVertexArrays(1, &this->circleVAO);
}

void FishRenderer::renderFishBody(const Fish& fish, Shader& circleShader, Shader& outlineShader){

    glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f * this->aspect_ratio, 20.0f * this->aspect_ratio, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    // render the circles

    glBindVertexArray(this->circleVAO);
    //
    // for(Joint joint : fish.joints){
    //     model = glm::mat4(1.0f);
    //     model = glm::translate(model, glm::vec3(joint.Center, 0.0f));
    //     model = glm::scale(model, glm::vec3(joint.circleRadius, joint.circleRadius, 1.0f));
    //
    //     circleShader.use();
    //     circleShader.setMat4("projection", projection);
    //     circleShader.setMat4("model", model);
    //     circleShader.setFloat("r", joint.circleRadius);
    //
    //     glDrawArrays(GL_TRIANGLES, 0, 6);
    // }

    // rendering just the first and last circle

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(fish.joints[0].Center, 0.0f));
    model = glm::scale(model, glm::vec3(fish.joints[0].circleRadius, fish.joints[0].circleRadius, 1.0f));

    circleShader.use();
    circleShader.setMat4("projection", projection);
    circleShader.setMat4("model", model);
    circleShader.setFloat("r", fish.joints[0].circleRadius);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(fish.joints[fish.numOfJoints-1].Center, 0.0f));
    model = glm::scale(model, glm::vec3(fish.joints[fish.numOfJoints-1].circleRadius, fish.joints[fish.numOfJoints-1].circleRadius, 1.0f));

    circleShader.use();
    circleShader.setMat4("projection", projection);
    circleShader.setMat4("model", model);
    circleShader.setFloat("r", fish.joints[fish.numOfJoints-1].circleRadius);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render the outline

    glBindVertexArray(this->outlineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->outlineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * fish.numOfJoints * sizeof(float), fish.outline_vertices);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(fish.joints[0].Center, 0.0f));

    outlineShader.use();
    outlineShader.setMat4("projection", projection);
    outlineShader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * fish.numOfJoints);

    glBindVertexArray(0);

}

void FishRenderer::renderFishBackFin(const Fish& fish, Shader& finShader){

    Joint j1 = fish.joints[fish.numOfJoints/4];
    Joint j2 = fish.joints[fish.numOfJoints/2 + 1];

    // this->back_fin_bezier.DrawBezierFilled(8, j1.Center, glm::vec2(0.0f), 
    //                                        j2.Center - j1.Center, -2.5f * j1.moveDirection, finShader);
    // this->back_fin_bezier.DrawBezierFilled(10, j1.Center, glm::vec2(0.0f), 
    //                                        j2.Center - j1.Center, -2.5f * j1.moveDirection * glm::vec2(-0.3f), finShader);

    glm::vec2 controlPoint = -2.5f * j1.moveDirection;

    glm::vec2 controlDirection = glm::normalize(controlPoint - (j1.Center+j2.Center)/2.0f);

    glm::vec2 controlPoint2 = controlPoint + 2.5f * controlDirection;

    // Cannot figur out how to get a good controlPoint2 value

    this->back_fin_bezier.DrawCrescentBezierFilled(3, j1.Center, glm::vec2(0.0f), j2.Center - j1.Center, controlPoint, controlPoint2, finShader);
}

















