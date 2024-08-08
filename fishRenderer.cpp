#include "fishRenderer.h"
#include <GLFW/glfw3.h>
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
    glDeleteVertexArrays(1, &this->circleVAO);
}

void FishRenderer::renderFishBody(const Fish& fish, Shader& circleShader, Shader& outlineShader){

    glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f * this->aspect_ratio, 20.0f * this->aspect_ratio, -1.0f, 1.0f);
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

    // model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(fish.tail_fin_joint.Center, 0.0f));
    // model = glm::scale(model, glm::vec3(fish.tail_fin_joint.circleRadius, fish.tail_fin_joint.circleRadius, 1.0f));
    //
    // circleShader.use();
    // circleShader.setMat4("projection", projection);
    // circleShader.setMat4("model", model);
    // circleShader.setFloat("r", fish.tail_fin_joint.circleRadius);
    // glDrawArrays(GL_TRIANGLES, 0, 6);

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

void FishRenderer::renderFishBackFin(const Fish& fish, Shader& finShader){  // This needs a lot of clean up

    Joint j1 = fish.joints[fish.numOfJoints/4];
    Joint j2 = fish.joints[fish.numOfJoints/2 + 1];

    glm::vec2 controlPoint = -2.5f * j1.moveDirection;

    glm::vec2 controlDirection = glm::normalize(controlPoint + j1.Center - (j1.Center+j2.Center)/2.0f);

    glm::vec2 controlPoint2 = controlPoint + 1.0f * controlDirection;

    // Draw back fin
    this->fin_bezier.DrawCrescentBezierFilled(8, j1.Center, glm::vec2(0.0f), j2.Center - j1.Center, controlPoint, controlPoint2, finShader);

    glm::vec2 point_on_circle = glm::vec2(glm::normalize(glm::cross(glm::vec3(fish.joints[fish.numOfJoints-1].moveDirection, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)))) * fish.joints[fish.numOfJoints-1].circleRadius;

    // glm::vec2 tail_fin_start = fish.joints[fish.numOfJoints-1].Center + point_on_circle;
    glm::vec2 tail_fin_start = fish.joints[fish.numOfJoints-1].Center;
    // glm::vec2 tail_fin_end = fish.joints[fish.numOfJoints-1].Center - point_on_circle;
    glm::vec2 tail_fin_end = fish.tail_fin_joints[1].Center;
    // this->fin_bezier.DrawBezierFilled(8, fish.joints[fish.numOfJoints-1] + , glm::vec2 p1, glm::vec2 p2, glm::vec2 controlPoint, Shader &shader)
    // glm::vec2 tail_fin_control = fish.tail_fin_joints[1].Center - fish.joints[fish.numOfJoints-1].Center;
    glm::vec2 tail_fin_control1 = fish.tail_fin_joints[0].Center;
    glm::vec2 tail_fin_control_offset_dir = tail_fin_control1 - (tail_fin_start+tail_fin_end)/2.0f; 
    tail_fin_control1 = tail_fin_control1 + tail_fin_control_offset_dir * 1.5f; 
    
    glm::vec2 tail_fin_control2 = tail_fin_control1 + tail_fin_control_offset_dir * 3.0f;
    // float sin_offset = (std::sin(glfwGetTime()) * 0.5 + 0.5) * 1.5f;

    // tail_fin_control_offset_dir += glm::vec2(glm::normalize(glm::cross(glm::vec3(tail_fin_control_offset_dir, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)))) * sin_offset;

    // this->fin_bezier.DrawBezierFilled(8, tail_fin_start, glm::vec2(0.0f), tail_fin_end - tail_fin_start, tail_fin_control, finShader);

    this->fin_bezier.DrawCrescentBezierFilled(8, tail_fin_start, glm::vec2(0.0f), tail_fin_end - tail_fin_start,
                                              tail_fin_control1 - tail_fin_start, tail_fin_control2 - tail_fin_start, finShader);
}

















