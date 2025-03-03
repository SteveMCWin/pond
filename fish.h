#ifndef FISH_H
#define FISH_H

#include <glad/glad.h>
#include "global.h"
#include <cstdlib>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

struct Joint{
    glm::vec2 Center;
    glm::vec2 moveDirection;
    float desiredDistance;
    float circleRadius;
    
    // Joint* jointConnectedTo;
    Joint(glm::vec2 c = glm::vec2(0.0f), float d = 1.0f, float r = 1.0f, glm::vec2 m = glm::normalize(glm::circularRand(1.0f))){
        Center = c;
        desiredDistance = d;
        circleRadius = r;
        moveDirection = m;
    }
};

class Fish{
public:
    std::vector<glm::vec2> hit_checks;
    std::vector<Joint> joints;
    Joint tail_fin_joints[2];
    unsigned int numOfJoints;
    float moveSpeed;
    // should add a reference to the center of the head joint so you don't have to type fish.joints[0].Center
    // glm::vec2& position = this->joints[0].Center;

    unsigned int num_of_hit_checks = 13;
    int hit_checks_result;
    float sightRange;
    int fishID;

    glm::ivec2 box_coords;

    glm::vec3 bodyColor;
    glm::vec3 finColor;
    glm::vec3 eyeColor;

    std::vector<float> outline_vertices;
    float tex_coords[(Global::numberOfJoints + 2) * 2 * 2];

     Fish(glm::vec2* centers, float* distances, float* radii, int id, float speed = 20.0f, float sRange = Global::fishSightRange,
          glm::vec3 bColor = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 fColor = glm::vec3(0.96f, 0.43f, 0.23f), glm::vec3 eColor = glm::vec3(0.1f, 0.2f, 0.2f));
    ~Fish();

    void updateMoveDir(glm::vec2 new_move_dir);
    void Move(float delta_time);

private:
    const float aspect_ratio = Global::aspectRatio;
    const float hit_check_angle = 120;
    float hit_check_distance;
    glm::vec2 next_move_dir;

    void updateJoints();
    void updateHitChecks();
    void print_outline_data();
};


#endif // !FISH_H
