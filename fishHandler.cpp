#include "fishHandler.h"
#include <iostream>

FishHandler::FishHandler(){}

void FishHandler::addFish(Fish& fish){

    this->allFish.push_back(fish);
}

void FishHandler::addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int id, int numOfHitChecks, float speed){

    this->allFish.push_back(Fish(numOfJoints, centers, distances, radii, id, numOfHitChecks, speed));
}

glm::vec2 FishHandler::calcFishMoveDir(Fish& fish){

    glm::vec2 edgeEvasionDir = Global::CalculateNormal(fish.joints[0].moveDirection) * (float)fish.hit_checks_result;

    glm::vec2 separationDir = glm::vec2(0.0f);
    glm::vec2 alignmentDir = glm::vec2(0.0f);
    glm::vec2 cohesionDir = glm::vec2(0.0f);
    int counter = 0;

    for(Fish& f : allFish){
        if(fish.fishID != f.fishID){
            if(glm::length(fish.joints[0].Center - f.joints[0].Center) < fish.sightRange){
                separationDir += fish.joints[0].Center - f.joints[0].Center;
                alignmentDir  += f.joints[0].moveDirection;
                cohesionDir   += f.joints[0].Center;
                counter++;
            }
        }
    }

    if(counter){
        separationDir = glm::normalize(separationDir/(float)counter);
        alignmentDir  = glm::normalize(alignmentDir/(float)counter - fish.joints[0].moveDirection);
        cohesionDir   = glm::normalize(cohesionDir/(float)counter  - fish.joints[0].moveDirection);
    }

    // glm::vec2 separationDir = calcSeparation(fish);
    // glm::vec2 separationOffset = glm::normalize(separationDir - (separationDir == glm::vec2(0.0f) ? glm::vec2(0.0f) : fish.joints[0].moveDirection));

    return edgeEvasionDir*edgeEvasionIntensity + separationDir*separationIntensity + alignmentDir*alignmentIntensity + cohesionDir*cohesionIntensity;
}
