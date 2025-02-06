#include "fish.h"
#include "global.h"
#include <glm/detail/func_geometric.hpp>
#include <ostream>

Fish::Fish(glm::vec2* centers, float* distances, float* radii, int id, float speed, float sRange,
           glm::vec3 bColor, glm::vec3 fColor, glm::vec3 eColor){

    this->numOfJoints = Global::numberOfJoints;
    this->outline_vertices.resize(4*this->numOfJoints);

    for(int i = 0; i < this->numOfJoints; i++){
        Joint j = Joint(centers[i], distances[i], radii[i]);
        this->joints.push_back(j);
    }

    this->tail_fin_joints[0] = this->joints[this->numOfJoints-1];
    this->tail_fin_joints[0].desiredDistance = 1.5f;
    this->tail_fin_joints[1] = this->tail_fin_joints[0];
    this->tail_fin_joints[1].desiredDistance = 1.5f;

    this->fishID = id;
    this->moveSpeed = speed;
    this->sightRange = sRange;
    this->hit_checks_result = 0;

    this->hit_checks.resize(this->num_of_hit_checks);
    this->hit_check_distance = 7.0f * radii[0];
    this->updateHitChecks();

    this->bodyColor = bColor;
    this->finColor  = fColor;
    this->eyeColor  = eColor;

    for(int i = 0; i < this->numOfJoints; i++){
        this->tex_coords[i*4  ] = static_cast<float>(i)/static_cast<float>(this->numOfJoints);
        this->tex_coords[i*4+1] = 0.1;
        this->tex_coords[i*4+2] = static_cast<float>(i)/static_cast<float>(this->numOfJoints);
        this->tex_coords[i*4+3] = 0.9;
    }
}

Fish::~Fish(){
    // this->outline_vertices.resize(0);
    this->outline_vertices.clear();         // this probably isn't neccessary but oh well
    this->outline_vertices.shrink_to_fit();
}

void Fish::Move(glm::vec2 direction, float delta_time){

    // moves the head of the fish in the direction passed into the function, then updates the position of the rest of the joints
    this->joints[0].moveDirection = glm::normalize(direction);
    this->joints[0].Center += this->joints[0].moveDirection * this->moveSpeed * delta_time;

    // also calculates the vertices of the outline
    glm::vec2 point = Global::CalculateNormal(this->joints[0].moveDirection);
    this->outline_vertices[0] = point.x * this->joints[0].circleRadius;
    this->outline_vertices[1] = point.y * this->joints[0].circleRadius;

    point = -point;
    this->outline_vertices[2] = point.x * this->joints[0].circleRadius;
    this->outline_vertices[3] = point.y * this->joints[0].circleRadius;

    this->updateJoints();
    this->updateHitChecks();
}

void Fish::updateJoints(){

    // used to update the position of all of the joints (except the first one, which is updated by the move function) and updates the outline vertices
    for(int i = 1; i < this->numOfJoints; i++){
        float distance_between_joints = glm::distance(this->joints[i-1].Center, this->joints[i].Center);
        float desired_distance = this->joints[i].desiredDistance;
        this->joints[i].moveDirection = glm::normalize(this->joints[i-1].Center - this->joints[i].Center);
        if(distance_between_joints - desired_distance > 1e-2 || distance_between_joints - desired_distance< 1e-3){
            this->joints[i].Center += this->joints[i].moveDirection * (distance_between_joints - desired_distance);
        }

        // The outline vertex data is positioned in relation to the center of the head

        glm::vec2 pointOnCircle = Global::CalculateNormal(this->joints[i].moveDirection);
        glm::vec2 relativePosition = this->joints[i].Center - this->joints[0].Center;
        this->outline_vertices[4*i]   = relativePosition.x + pointOnCircle.x * this->joints[i].circleRadius;
        this->outline_vertices[4*i+1] = relativePosition.y + pointOnCircle.y * this->joints[i].circleRadius;

        pointOnCircle = -pointOnCircle;
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

void Fish::updateHitChecks(){   // could move this to a compute shader

    // num_of_hit_checks
    // degreeChange
    // function for rotating a 2d vector
    // joints[0].moveDirection
    // joints[0].Center
    // hit_check_distance
    // Global::screenHalfSize
    // result

    hit_checks_result = 0;

    float degreeChange = this->hit_check_angle/(this->hit_checks.size()-1);

    for(int i = -(this->hit_checks.size()/2), j = 0; j < this->hit_checks.size(); j++, i++){

        this->hit_checks[j] = Global::rotateVector(this->joints[0].moveDirection, -degreeChange * i);
        
        glm::vec2 hit_check_world_pos = this->joints[0].Center + this->hit_checks[j] * hit_check_distance;

        // The if checks if the position of the hit check is outside a square with round corners, the exponent (4 in this case) determines the roundness (smaller exp -> rounder)
        // the result of 1.0 is just on the edge of the window, less than is inside, more than is on the outside
        if( powf(std::abs(hit_check_world_pos.x/Global::screenHalfSize), 4) +
            powf(std::abs(hit_check_world_pos.y/(Global::screenHalfSize*Global::aspectRatio)), 4) >= 1.0){

                hit_checks_result += (i < 0) ? -1 : 1;
        }


    }
}

