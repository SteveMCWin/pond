#include "fish.h"

Fish::Fish(int jointNum, glm::vec2* centers, float* distances, float* radii, float speed){
    this->numOfJoints = jointNum;
    for(int i = 0; i < jointNum; i++){
        Joint j = Joint(centers[i], distances[i], radii[i]);
        this->joints.push_back(j);
        // std::cout << "Made joint number " << i+1 << std::endl;
        // std::cout << "joint " << i+1 << ":\t<" << centers[i].x << ", " << centers[i].y << ">" << std::endl;
    }
    this->moveSpeed = speed;
}

void Fish::Move(glm::vec2 direction){
    this->joints[0].moveDirection = glm::normalize(direction);
    this->joints[0].Center += this->joints[0].moveDirection * this->moveSpeed;
    // this->joints[0].Center.y *= this->aspect_ratio;
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
    }

}


