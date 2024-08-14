#include "fishRenderer.h"
#include "global.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/detail/func_geometric.hpp>
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
    glDeleteBuffers(1, &this->outlineVBO);
    glDeleteVertexArrays(1, &this->circleVAO);
    glDeleteVertexArrays(1, &this->outlineVAO);
}

glm::vec2 jointSidePoint(Joint& j){
    return glm::vec2(glm::normalize(glm::cross(glm::vec3(j.moveDirection, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)))) * j.circleRadius;
}

void FishRenderer::renderFishBody(const Fish& fish, Shader& circleShader, Shader& outlineShader){

    glm::mat4 projection = Global::projectionMatrix;
    glm::mat4 model = glm::mat4(1.0f);

    // render the circles

    glBindVertexArray(this->circleVAO);

    // for(Joint joint : fish.joints){
    //     model = glm::mat4(1.0f);
    //     model = glm::translate(model, glm::vec3(joint.Center, 0.0f));
    //     model = glm::scale(model, glm::vec3(joint.circleRadius, joint.circleRadius, 1.0f));
    //
    //     circleShader.use();
    //     circleShader.setMat4("projection", projection);
    //     circleShader.setMat4("model", model);
    //     circleShader.setVec3("color", 0.2f, 0.5f, 0.7f);
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
    circleShader.setVec3("color", 0.2f, 0.5f, 0.7f);
    circleShader.setFloat("r", fish.joints[0].circleRadius);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(fish.joints[fish.numOfJoints-1].Center, 0.0f));
    model = glm::scale(model, glm::vec3(fish.joints[fish.numOfJoints-1].circleRadius, fish.joints[fish.numOfJoints-1].circleRadius, 1.0f));

    circleShader.use();
    circleShader.setMat4("projection", projection);
    circleShader.setMat4("model", model);
    circleShader.setVec3("color", 0.2f, 0.5f, 0.7f);
    circleShader.setFloat("r", fish.joints[fish.numOfJoints-1].circleRadius);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render the outline

    glBindVertexArray(this->outlineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->outlineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * fish.numOfJoints * sizeof(float), fish.outline_vertices.data());

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(fish.joints[0].Center, 0.0f));

    outlineShader.use();
    outlineShader.setMat4("projection", projection);
    outlineShader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * fish.numOfJoints);

    glBindVertexArray(0);

}

void FishRenderer::renderFishBackFin(const Fish& fish, Shader& finShader){  // This needs a lot of clean up

    Joint j1 = fish.joints[fish.numOfJoints/4];
    Joint j2 = fish.joints[fish.numOfJoints/2 + 1];

    glm::vec2 controlPoint = -2.5f * j1.moveDirection;

    glm::vec2 controlDirection = glm::normalize(controlPoint + j1.Center - (j1.Center+j2.Center)/2.0f);

    glm::vec2 controlPoint2 = controlPoint + 1.0f * controlDirection;

    // Draw back fin
    this->fin_bezier.DrawCrescentBezierFilled(8, j1.Center, glm::vec2(0.0f), j2.Center - j1.Center, controlPoint, controlPoint2, finShader);
}

void FishRenderer::renderFishTailFin(const Fish& fish, Shader& finShader){

    glm::vec2 tail_fin_start = fish.joints[fish.numOfJoints-1].Center;
    glm::vec2 tail_fin_end = fish.tail_fin_joints[1].Center;

    glm::vec2 tail_fin_control1 = fish.tail_fin_joints[0].Center;
    glm::vec2 tail_fin_control_offset_dir = tail_fin_control1 - (tail_fin_start+tail_fin_end)/2.0f; 

    // float sin_offset = (std::sin(glfwGetTime()) * 0.5 + 1.0);
    // tail_fin_control_offset_dir *= sin_offset;

    tail_fin_control1 = tail_fin_control1 + tail_fin_control_offset_dir * 1.5f; 
    glm::vec2 tail_fin_control2 = tail_fin_control1 + tail_fin_control_offset_dir * 5.0f;

    this->fin_bezier.DrawCrescentBezierFilled(8, tail_fin_start, glm::vec2(0.0f), tail_fin_end - tail_fin_start,
                                              tail_fin_control1 - tail_fin_start, tail_fin_control2 - tail_fin_start, finShader);
}

float angleBetweenVectors(glm::vec2 v1, glm::vec2 v2){
    float angle = std::acos(glm::dot(v1, v2));
    if(v1.y < 0.0f){
        angle *= -1.0f;
    }
    return angle;
}

void FishRenderer::renderOvals(glm::vec2 position, glm::vec2 offset, float rotationAngle, glm::vec2 scale, Shader& shader, glm::vec3 color = glm::vec3(0.1f, 0.9f, 0.3f), float r = 0.5f){

    glm::mat4 projection = Global::projectionMatrix;
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position + offset, 0.0f));
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("model", model);
    shader.setVec3("color", color);
    shader.setFloat("r", r);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FishRenderer::renderFishSideFins(const Fish& fish, glm::vec2 frontScale, glm::vec2 backScale, Shader& finShader){

    Joint frontFinsJoint = fish.joints[fish.numOfJoints/6];   
    Joint backFinsJoint  = fish.joints[fish.numOfJoints/2+1];   

    // glm::vec2 diff = glm::normalize(frontFinsJoint.moveDirection - fish.joints[0].moveDirection);
    // std::cout << "Difference: <" << diff.x << ", " << diff.y << ">\n";

    glm::vec2 frontOffset = jointSidePoint(frontFinsJoint);
    glm::vec2 backOffset  = jointSidePoint(backFinsJoint);

    float rightFinAngle = angleBetweenVectors(frontFinsJoint.moveDirection, glm::vec2(1.0f, 0.0f)) - 3.0f*pi/4.0f;
    float leftFinAngle  = rightFinAngle + 6.0f*pi/4.0f;

    glBindVertexArray(this->circleVAO);

    this->renderOvals(frontFinsJoint.Center, frontOffset, rightFinAngle, frontScale, finShader);
    this->renderOvals(frontFinsJoint.Center, -frontOffset, leftFinAngle, frontScale, finShader);

    rightFinAngle = angleBetweenVectors(backFinsJoint.moveDirection, glm::vec2(1.0f, 0.0f)) - 7.0f*pi/8.0f;
    leftFinAngle  = rightFinAngle + 14.0f*pi/8.0f;

    this->renderOvals(backFinsJoint.Center, backOffset, rightFinAngle, backScale, finShader);
    this->renderOvals(backFinsJoint.Center, -backOffset, leftFinAngle, backScale, finShader);

    glBindVertexArray(0);
}

void FishRenderer::renderFishEyes(const Fish& fish, glm::vec2 scale, Shader& circleShader){

    Joint headJoint = fish.joints[0];

    glm::vec2 offset = jointSidePoint(headJoint) * 0.85f;

    float rightEyeAngle = angleBetweenVectors(headJoint.moveDirection, glm::vec2(1.0f, 0.0f)) - pi/2.0f + pi/20.0f;
    float leftEyeAngle  = angleBetweenVectors(headJoint.moveDirection, glm::vec2(1.0f, 0.0f)) + pi/2.0f - pi/20.0f;

    glBindVertexArray(this->circleVAO);

    this->renderOvals(headJoint.Center, offset, rightEyeAngle, scale * 1.4f, circleShader, glm::vec3(0.2f, 0.5f, 0.7f));   // right eye
    this->renderOvals(headJoint.Center, offset, rightEyeAngle, scale, circleShader, glm::vec3(0.05f, 0.1f, 0.1f)*2.0f);   // right eye

    this->renderOvals(headJoint.Center, -offset, leftEyeAngle, scale * 1.4f, circleShader, glm::vec3(0.2f, 0.5f, 0.7f));   // left eye
    this->renderOvals(headJoint.Center, -offset, leftEyeAngle, scale, circleShader, glm::vec3(0.05f, 0.1f, 0.1f)*2.0f);   // left  eye

    glBindVertexArray(0);

}















