#include "fish.h"
#include <glm/detail/func_geometric.hpp>

Fish::Fish(int jointNum, glm::vec2* centers, float* distances, float* radii, float speed){
    this->numOfJoints = jointNum;
    this->outline_vertices = new float[4*jointNum];
    for(int i = 0; i < jointNum; i++){
        Joint j = Joint(centers[i], distances[i], radii[i]);
        this->joints.push_back(j);

        // this->outline_vertices[4*i] = 0.0f;
        // this->outline_vertices[4*i+1] = 0.0f;
        // this->outline_vertices[4*i+2] = 0.0f;
        // this->outline_vertices[4*i+3] = 0.0f;
        // std::cout << "Made joint number " << i+1 << std::endl;
        // std::cout << "joint " << i+1 << ":\t<" << centers[i].x << ", " << centers[i].y << ">" << std::endl;
    }
    this->tail_fin_joint = this->joints[jointNum-1];
    this->tail_fin_joint.desiredDistance *= 2.0f;
    this->moveSpeed = speed;
}

Fish::~Fish(){
    delete[] this->outline_vertices;
}

void Fish::Move(glm::vec2 direction){
    this->joints[0].moveDirection = glm::normalize(direction);
    this->joints[0].Center += this->joints[0].moveDirection * this->moveSpeed;

    glm::vec3 point = glm::normalize(glm::cross(glm::vec3(this->joints[0].moveDirection, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    this->outline_vertices[0] = point.x * this->joints[0].circleRadius;
    this->outline_vertices[1] = point.y * this->joints[0].circleRadius;

    // point = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(this->joints[0].moveDirection, 0.0f)));    // i think here I can just do point = -point;
    point = -point;
    this->outline_vertices[2] = point.x * this->joints[0].circleRadius;
    this->outline_vertices[3] = point.y * this->joints[0].circleRadius;

    this->updateJoints();
}

void Fish::updateJoints(){

    for(int i = 1; i < this->numOfJoints; i++){
        float distance_between_joints = glm::distance(this->joints[i-1].Center, this->joints[i].Center);
        float desired_distance = this->joints[i].desiredDistance;
        this->joints[i].moveDirection = glm::normalize(this->joints[i-1].Center - this->joints[i].Center);
        if(distance_between_joints - desired_distance > 1e-2 || distance_between_joints - desired_distance< 1e-3){
            this->joints[i].Center += this->joints[i].moveDirection * (distance_between_joints - desired_distance);
        }

        // The outline vertex data is positioned in relation to the center of the head

        glm::vec3 pointOnCircle = glm::normalize(glm::cross(glm::vec3(this->joints[i].moveDirection, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        glm::vec2 relativePosition = this->joints[i].Center - this->joints[0].Center;
        this->outline_vertices[4*i]   = relativePosition.x + pointOnCircle.x * this->joints[i].circleRadius;
        this->outline_vertices[4*i+1] = relativePosition.y + pointOnCircle.y * this->joints[i].circleRadius;

        pointOnCircle = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(this->joints[i].moveDirection, 0.0f)));
        this->outline_vertices[4*i+2] = relativePosition.x + pointOnCircle.x * this->joints[i].circleRadius;
        this->outline_vertices[4*i+3] = relativePosition.y + pointOnCircle.y * this->joints[i].circleRadius;
       
    }

    if(this->tail_fin_joint.desiredDistance - glm::distance(this->tail_fin_joint.Center, this->joints[this->numOfJoints-1].Center) < 1e-2){
        tail_fin_joint.moveDirection = glm::normalize(joints[numOfJoints-1].Center - tail_fin_joint.Center);
        tail_fin_joint.Center += tail_fin_joint.moveDirection * (glm::distance(tail_fin_joint.Center, joints[numOfJoints-1].Center) - tail_fin_joint.desiredDistance);
    }

}


