#ifndef FISH_H
#define FISH_H

#include "glad.h"
#include "global.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

struct Joint{
    glm::vec2 Center;
    glm::vec2 moveDirection;
    float desiredDistance;
    float circleRadius;
    
    // Joint* jointConnectedTo;
    Joint(glm::vec2 c = glm::vec2(0.0f), float d = 1.0f, float r = 1.0f, glm::vec2 m = glm::vec2(0.0f, 1.0f)){
        Center = c;
        desiredDistance = d;
        circleRadius = r;
        moveDirection = m;
    }
};

class Fish{
public:
    std::vector<Joint> joints;
    std::vector<glm::vec2> hit_checks;
    Joint tail_fin_joints[2];
    float moveSpeed;
    unsigned int numOfJoints;

    float* outline_vertices;
    // std::vector<float> outline_vertices;

     Fish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int numOfHitChecks = 3, float speed = 0.2f);
    ~Fish();

    void Move(glm::vec2 direction);

private:
    const float aspect_ratio = Global::aspectRatio;
    const float hit_check_angle = 120;
    float hit_check_distance;

    void updateJoints();
    void updateHitChecks();
    glm::vec2 rotateVector(glm::vec2& vector, float degrees);
    // rendering will be handled by a separate class
};


#endif // !FISH_H
