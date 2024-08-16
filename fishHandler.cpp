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

    glm::vec2 separationDir = calcSeparation(fish);
    // glm::vec2 separationOffset = glm::normalize(separationDir - (separationDir == glm::vec2(0.0f) ? glm::vec2(0.0f) : fish.joints[0].moveDirection));

    return separationDir * separationIntensity;
}

glm::vec2 FishHandler::calcSeparation(Fish& fish){

    glm::vec2 moveDir = glm::vec2(0.0f);
    int counter = 0;

    for(Fish& f : allFish){
        if(fish.fishID != f.fishID){
            if(glm::length(fish.joints[0].Center - f.joints[0].Center) < fish.sightRange){
                moveDir += fish.joints[0].Center - f.joints[0].Center;
                counter++;
            }
        }
    }

    if(!counter)
        return glm::vec2(0.0f);

    return glm::normalize(moveDir/(float)counter);
}

glm::vec2 FishHandler::calcAlignment(Fish& fish){

    return glm::vec2();
}

glm::vec2 FishHandler::calcCohesion(Fish& fish){

    return glm::vec2();
}
