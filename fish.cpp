#include "fish.h"
#include <glm/detail/func_geometric.hpp>
#include <ostream>

Fish::Fish(int jointNum, glm::vec2* centers, float* distances, float* radii, int numOfHitChecks, float speed){
    this->numOfJoints = jointNum;
    // this->outline_vertices = new float[4*jointNum];
    this->outline_vertices.resize(4*jointNum);
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
    this->tail_fin_joints[0] = this->joints[jointNum-1];
    this->tail_fin_joints[0].desiredDistance = 1.5f;
    this->tail_fin_joints[1] = this->tail_fin_joints[0];
    this->tail_fin_joints[1].desiredDistance = 1.5f;
    this->moveSpeed = speed;
    this->hit_checks_result = 0;

    numOfHitChecks += (numOfHitChecks % 2) ? 0 : 1;

    this->hit_checks.resize(numOfHitChecks);
    this->hit_check_distance = 7.0f * radii[0];
    this->updateHitChecks();

    std::cout << "Fish speed: " << this->moveSpeed << std::endl;
}

Fish::~Fish(){
    // this->outline_vertices.resize(0);
    this->outline_vertices.clear();
    this->outline_vertices.shrink_to_fit();
}

void Fish::Move(glm::vec2 direction, float delta_time){
    this->joints[0].moveDirection = glm::normalize(direction);
    this->joints[0].Center += this->joints[0].moveDirection * this->moveSpeed * delta_time;

    glm::vec3 point = glm::normalize(glm::cross(glm::vec3(this->joints[0].moveDirection, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    this->outline_vertices[0] = point.x * this->joints[0].circleRadius;
    this->outline_vertices[1] = point.y * this->joints[0].circleRadius;

    // point = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(this->joints[0].moveDirection, 0.0f)));    // i think here I can just do point = -point;
    point = -point;
    this->outline_vertices[2] = point.x * this->joints[0].circleRadius;
    this->outline_vertices[3] = point.y * this->joints[0].circleRadius;

    this->updateHitChecks();
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

    if(this->tail_fin_joints[0].desiredDistance - glm::distance(this->tail_fin_joints[0].Center, this->joints[this->numOfJoints-1].Center) < 1e-2){
        tail_fin_joints[0].moveDirection = glm::normalize(joints[numOfJoints-1].Center - tail_fin_joints[0].Center);
        tail_fin_joints[0].Center += tail_fin_joints[0].moveDirection * (glm::distance(tail_fin_joints[0].Center, joints[numOfJoints-1].Center) - tail_fin_joints[0].desiredDistance);
    }

    if(this->tail_fin_joints[1].desiredDistance - glm::distance(this->tail_fin_joints[0].Center, this->tail_fin_joints[1].Center) < 1e-2){
        tail_fin_joints[1].moveDirection = glm::normalize(tail_fin_joints[0].Center - tail_fin_joints[1].Center);
        tail_fin_joints[1].Center += tail_fin_joints[1].moveDirection * (glm::distance(tail_fin_joints[1].Center, tail_fin_joints[0].Center) - tail_fin_joints[1].desiredDistance);
    }
}

void Fish::updateHitChecks(){

    hit_checks_result = 0;

    float degreeChange = hit_check_angle/(this->hit_checks.size()-1);

    // std::cout << "hit_check[1].y: " << -(this->joints[1].Center.y + this->hit_checks[1].y * hit_check_distance) << ">\n";
    // std::cout << "hit_check_itself <" << this->hit_checks[1].x << ", " << -this->hit_checks[1].y << ">\n";

    for(int i = -(this->hit_checks.size()/2), j = 0; j < this->hit_checks.size(); j++, i++){

        this->hit_checks[j] = rotateVector(this->joints[0].moveDirection, -degreeChange * i);
        
        glm::vec2 hit_check_world_pos = this->joints[0].Center + this->hit_checks[j] * hit_check_distance;
        if(  hit_check_world_pos.x > -Global::bottomLeftCorner.x ||
             hit_check_world_pos.x <  Global::bottomLeftCorner.x ||
            -hit_check_world_pos.y >  Global::bottomLeftCorner.y ||
            -hit_check_world_pos.y < -Global::bottomLeftCorner.y){

                hit_checks_result += (i < 0) ? 1 : -1;
                // std::cout << "hi" << std::endl;
        }

    }

    // if(-(this->joints[0].Center.y + this->hit_checks[this->hit_checks.size()/2].y * hit_check_distance) < Global::bottomLeftCorner.y + Global::screenHalfSize*2){
    //     std::cout << "Hit_Check[" << this->hit_checks.size()/2 << "]" << std::endl;
    // }
    // else{
    //     std::cout << "Nope" << std::endl;
    // }
    // std::cout << "\rhit_check_result: " << hit_checks_result << std::flush;

}

glm::vec2 Fish::rotateVector(glm::vec2& vector, float angle){

    angle = Global::DegToRad(angle);

    float new_x = std::cos(angle)*vector.x - std::sin(angle)*vector.y;
    float new_y = std::sin(angle)*vector.x + std::cos(angle)*vector.y;

    return glm::normalize(glm::vec2(new_x, new_y));
}

