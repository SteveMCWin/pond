#include "fishHandler.h"
#include "global.h"
#include <GLFW/glfw3.h>
#include <glm/detail/func_geometric.hpp>
#include <iostream>

FishHandler::FishHandler(){}

void FishHandler::addFish(Fish& fish){

    this->allFish.push_back(fish);
}

void FishHandler::addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int id, int numOfHitChecks, float speed){

    this->allFish.push_back(Fish(numOfJoints, centers, distances, radii, id, numOfHitChecks, speed));
}

glm::vec2 FishHandler::calcFishMoveDir(Fish& fish, float delta_time){

    glm::vec2 resultDir = fish.joints[0].moveDirection;

    glm::vec2 separationVec = glm::vec2(0.0f);
    glm::vec2 alignmentVec = glm::vec2(0.0f);
    glm::vec2 cohesionVec = glm::vec2(0.0f);

    int counter = 0;
    int separationCounter = 0;

    for(Fish& f : allFish){
        if(fish.fishID != f.fishID){

            if(glm::length(fish.joints[0].Center - f.joints[0].Center) < 0.4 * fish.sightRange && glm::dot(resultDir, f.joints[0].Center - fish.joints[0].Center) > -0.8){
                separationVec += fish.joints[0].Center - f.joints[0].Center;
                alignmentVec  += f.joints[0].moveDirection;
                cohesionVec   += f.joints[0].Center;
                separationCounter++;
                counter++;
            }
            else if(glm::length(fish.joints[0].Center - f.joints[0].Center) < 0.8 * fish.sightRange && glm::dot(resultDir, f.joints[0].Center - fish.joints[0].Center) > -0.8){
                alignmentVec  += f.joints[0].moveDirection;
                cohesionVec   += f.joints[0].Center;
                counter++;
            }
        }
    }

    resultDir = Global::rotateVector(resultDir, fish.hit_checks_result * edgeEvasionIntensity * delta_time);

    if(separationCounter){
        // separationVec = separationVec/(float)separationCounter;

        float separationNum = glm::length(separationVec/(float)separationCounter);

        separationNum = 1.0f - separationNum/(0.4f * fish.sightRange);

        separationNum = powf(separationNum, 0.01f);

        float rotationDir;

        if(glm::cross(glm::vec3(resultDir, 0.0f), glm::vec3(separationVec, 0.0f)).z > 0)
            rotationDir =  1.0f;
        else
            rotationDir = -1.0f;

        resultDir = Global::rotateVector(resultDir, rotationDir * separationNum * separationIntensity * delta_time);
    }

    if(counter){
        alignmentVec  = alignmentVec/(float)counter;
        cohesionVec   = cohesionVec/(float)counter;

        float rotationDir;

        if(glm::cross(glm::vec3(resultDir, 0.0f), glm::vec3(alignmentVec, 0.0f)).z > 0)
            rotationDir =  1.0f;
        else
            rotationDir = -1.0f;

        // resultDir = Global::rotateVector(resultDir, rotationDir * glm::length(alignmentVec) * alignmentIntensity * delta_time);

        if(glm::cross(glm::vec3(resultDir, 0.0f), glm::vec3(cohesionVec, 0.0f)).z > 0)
            rotationDir =  1.0f;
        else
            rotationDir = -1.0f;

        // resultDir = Global::rotateVector(resultDir, rotationDir * glm::length(cohesionVec) * cohesionIntensity * delta_time);

    }

    resultDir = Global::rotateVector(resultDir, (float)std::sin((glfwGetTime() + fish.fishID * 7.0f) * 2.0f) * 50.0f * delta_time);

    return resultDir;
}



