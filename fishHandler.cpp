#include "fishHandler.h"

FishHandler::FishHandler(){}

void FishHandler::addFish(Fish& fish){

    this->allFish.push_back(fish);
}

void FishHandler::addFish(int numOfJoints, glm::vec2* centers, float* distances, float* radii, int numOfHitChecks, float speed){

    this->allFish.push_back(Fish(numOfJoints, centers, distances, radii, numOfHitChecks, speed));
}
