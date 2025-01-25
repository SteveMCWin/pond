#include "fishHandler.h"
#include "global.h"
#include <GLFW/glfw3.h>
#include <glm/detail/func_geometric.hpp>
#include <iostream>

FishHandler::FishHandler(){
    glGenBuffers(1, &this->ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, Global::numberOfFish * sizeof(hit_check_struct), NULL, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void FishHandler::addFish(Fish& fish){

    this->allFish.push_back(fish);
}

void FishHandler::addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int id, float speed){

    this->allFish.push_back(Fish(numOfJoints, centers, distances, radii, id, speed));
}

void FishHandler::calcFishHitChecks(){
    hit_check_struct hit_check_data[Global::numberOfFish];
    for(int i = 0; i < Global::numberOfFish; i++){
        // hit_check_data = {this->allFish[i].joints[0].moveDirection, this->allFish[i].joints[0].Center, };
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(hit_check_data), hit_check_data);
}

glm::vec2 FishHandler::calcFishMoveDir(Fish& fish, float delta_time){

    glm::vec2 resultDir = fish.joints[0].moveDirection; // the final direction that will be passed to the fish's Move function

    glm::vec2 separationVec = glm::vec2(0.0f);
    glm::vec2 alignmentVec = glm::vec2(0.0f);
    glm::vec2 cohesionVec = glm::vec2(0.0f);

    int counter = 0;    // counter used for alignment and cohesion
    int separationCounter = 0;  // counter used only for the separation

    float separationSight = 0.3f * fish.sightRange; // other fish inside this 'sight' field will be accounted for in the separation part of the boid simulation
    float otherSight = 1.0f * fish.sightRange;  // other fish inside this 'sight' field will be accounted for in the alignment and cohesion part of the boid simulation

    for(Fish& f : this->allFish){
        if(fish.fishID != f.fishID){
            // note that in the following statement another condition is that the fish is not right behind the one we are currently observing
            if(glm::length(fish.joints[0].Center - f.joints[0].Center) < separationSight && glm::dot(resultDir, f.joints[0].Center - fish.joints[0].Center) > -0.7){
                separationVec += fish.joints[0].Center - f.joints[0].Center;
                alignmentVec  += f.joints[0].moveDirection;
                cohesionVec   += f.joints[0].Center;
                separationCounter++;
                counter++;
            }
            else if(glm::length(fish.joints[0].Center - f.joints[0].Center) < otherSight && glm::dot(resultDir, f.joints[0].Center - fish.joints[0].Center) > -0.7){
                alignmentVec  += f.joints[0].moveDirection;
                cohesionVec   += f.joints[0].Center;
                counter++;
            }
        }
    }

    resultDir = Global::rotateVector(resultDir, fish.hit_checks_result * edgeEvasionIntensity * delta_time);    // try not to go off-screen

    if(separationCounter){

        float separationNum = glm::length(separationVec/(float)separationCounter);  // used to map the length of separationVec from [0, separationSight] to [0, 1] but in reverse

        separationNum = 1.0f - separationNum/separationSight;   // so if the length is closer to 0 it will be mapped closer to 1 so that if other fish are closer
                                                                // the observed fish will want to get away from them more
        separationNum = powf(separationNum, 0.01f); // map the onto a nice curve

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

        float alignmentAngleDelta = Global::angleOfVector(alignmentVec) - Global::angleOfVector(resultDir); // difference of the observed fish's move dir and the other fishes' move dir

        float alignmentFactor = (float)counter / (float)(this->allFish.size() - 1); // more fish around observed fish -> more influence on the observed fish's move direciton

        resultDir = Global::rotateVector(resultDir, alignmentAngleDelta * alignmentFactor * alignmentIntensity * delta_time);

        float rotationDir;

        if(glm::cross(glm::vec3(resultDir, 0.0f), glm::vec3(cohesionVec, 0.0f)).z > 0)
            rotationDir =  1.0f;
        else
            rotationDir = -1.0f;

        resultDir = Global::rotateVector(resultDir, rotationDir * glm::length(cohesionVec) * cohesionIntensity * delta_time);   // just go to the average position of close fish

    }

    resultDir = Global::rotateVector(resultDir, (float)std::sin((glfwGetTime() + fish.fishID * 7.0f) * 2.0f) * 50.0f * delta_time); // make the fish wiggle while swimming

    return resultDir;
}



