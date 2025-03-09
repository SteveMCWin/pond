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
    
    Joint(glm::vec2 c = glm::vec2(0.0f), float d = 1.0f, float r = 1.0f, glm::vec2 m = glm::normalize(glm::circularRand(1.0f))){
        Center = c;
        desiredDistance = d;
        circleRadius = r;
        moveDirection = m;
    }
};

struct Fish{
public:

    glm::vec2 hit_checks[NUM_OF_HIT_CHECKS];

    Joint joints[NUM_OF_JOINTS];

    Joint tail_fin_joints[2];

    int hit_checks_result;
    int fishID;

    glm::ivec2 box_coords;

    glm::vec3 bodyColor;    // CHANGE: GOTTA MOVE ALL OF THESE TO THE FISH HANDLER
    glm::vec3 finColor;
    glm::vec3 eyeColor;

    // *2 because vec2 has 2 floats
    float outline_vertices[NUM_OF_FISH_OUTLINE_VERTS * 2];
    float tex_coords[NUM_OF_FISH_OUTLINE_VERTS * 2];

     Fish();
     Fish(glm::vec2* centers, float* distances, float* radii, int id, glm::vec3 bColor = glm::vec3(1.0f, 1.0f, 1.0f),   // CHANGE: GOTTA FIX THIS CONSTRUCTOR AS WELl
          glm::vec3 fColor = glm::vec3(0.96f, 0.43f, 0.23f), glm::vec3 eColor = glm::vec3(0.1f, 0.2f, 0.2f));
    ~Fish();

    void updateMoveDir(glm::vec2 new_move_dir);
    void Move(float delta_time);

private:
    float hit_check_distance;
    glm::vec2 next_move_dir;

    void updateJoints();
    void updateHitChecks();
};


#endif // !FISH_H
