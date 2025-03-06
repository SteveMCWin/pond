#include "fishHandler.h"
#include "global.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdlib>
#include <glm/detail/func_geometric.hpp>
#include <iostream>

FishHandler::FishHandler(){

    int boxes_rows = FRUSTUM_HALF_WIDTH*Global::aspect_ratio/FISH_SIGHT_RANGE + 2;
    int boxes_cols = FRUSTUM_HALF_WIDTH/FISH_SIGHT_RANGE + 2;

    this->boxes = new box_struct*[boxes_rows];
    for(int i = 0; i < boxes_rows; i++){
        this->boxes[i] = new box_struct[boxes_cols];
    }

    for(int i = 0; i < boxes_rows; i++){
        for(int j = 0; j < boxes_cols; j++){
            this->boxes[i][j].fish_indexes = new int[number_of_fish];
            this->boxes[i][j].num_of_boxed_fish = 0;
        }
    }

}

FishHandler::~FishHandler(){
    
    int boxes_rows = FRUSTUM_HALF_WIDTH*Global::aspect_ratio/FISH_SIGHT_RANGE + 2;
    int boxes_cols = FRUSTUM_HALF_WIDTH/FISH_SIGHT_RANGE + 2;

    for(int i = 0; i < boxes_rows; i++){
        for(int j = 0; j < boxes_cols; j++){
            delete this->boxes[i][j].fish_indexes;
        }
        delete[] this->boxes[i];
    }

    delete[] this->boxes;

}

void FishHandler::createFish(){

    float distances[NUM_OF_JOINTS]   = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};   // desired distances from one circle center to another
    float radii[NUM_OF_JOINTS]       = {1.2f, 1.3f, 1.4f, 1.4f, 1.3f, 1.2f, 1.0f, 0.8f, 0.6f, 0.5f, 0.3f, 0.2f};   // radii of each visible circle
    glm::vec2 centers[NUM_OF_JOINTS] = {     // position of each circle center, the values get changed the moment the fish update joints func gets called so they can be 0.0
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
    };

    for(int i = 0; i < NUM_OF_JOINTS; i++){
        centers[0] = glm::vec2((Global::GetRandomFloat()*2.0f - 1.0f), Global::GetRandomFloat()*2.0f - 1.0f) * Global::bottomLeftCorner;
        this->addFish(centers, distances, radii, i);
    }
}

void FishHandler::addFish(Fish& fish){

    this->allFish.push_back(fish);
}

void FishHandler::addFish(glm::vec2* centers, float* distances, float* radii, int id, float speed){

    this->allFish.push_back(Fish(centers, distances, radii, id, speed));
}

void FishHandler::boxTheFish(){

    int boxes_rows = FRUSTUM_HALF_WIDTH*Global::aspect_ratio/FISH_SIGHT_RANGE + 2;
    int boxes_cols = FRUSTUM_HALF_WIDTH/FISH_SIGHT_RANGE + 2;

    // int box_row_index = std::clamp(fish.joints[0].Center.x/fish.sightRange + 1, 1, boxes_rows - 1);
    for(Fish& fish : this->allFish){
        int box_row_index = fish.joints[0].Center.x/fish.sightRange + 1;
        if(box_row_index < 1) box_row_index = 1;
        else if(box_row_index > boxes_rows - 2) box_row_index = boxes_rows - 2;
        int box_col_index = fish.joints[0].Center.x/fish.sightRange + 1;
        if(box_col_index < 1) box_col_index = 1;
        else if(box_col_index > boxes_rows - 2) box_col_index = boxes_rows - 2;
        this->boxes[box_row_index][box_col_index].fish_indexes[this->boxes[box_row_index][box_col_index].num_of_boxed_fish++] = fish.fishID;
        fish.box_coords.x = box_row_index;
        fish.box_coords.y = box_col_index;

    }
}

void FishHandler::resetBoxSizes(){

    int boxes_rows = FRUSTUM_HALF_WIDTH*Global::aspect_ratio/FISH_SIGHT_RANGE + 2;
    int boxes_cols = FRUSTUM_HALF_WIDTH/FISH_SIGHT_RANGE + 2;
    
    for(int i = 0; i < boxes_rows; i++){
        for(int j = 0; j < boxes_rows; j++){
            this->boxes[i][j].num_of_boxed_fish = 0;
        }
    }

}

glm::vec2 FishHandler::calcFishMoveDir(Fish& fish, float delta_time){

    // if(!fish.fishID){
    //     std::cout << "Current pos: " << fish.joints[0].Center.x << ", " << fish.joints[0].Center.y << std::endl;
    // }

    glm::vec2 resultDir = fish.joints[0].moveDirection; // the final direction that will be passed to the fish's Move function

    glm::vec2 separationVec = glm::vec2(0.0f);
    glm::vec2 alignmentVec = glm::vec2(0.0f);
    glm::vec2 cohesionVec = glm::vec2(0.0f);

    int counter = 0;    // counter used for alignment and cohesion
    int separationCounter = 0;  // counter used only for the separation

    float separationSight = 0.3f * fish.sightRange; // other fish inside this 'sight' field will be accounted for in the separation part of the boid simulation
    float otherSight = 1.0f * fish.sightRange;  // other fish inside this 'sight' field will be accounted for in the alignment and cohesion part of the boid simulation

    // for(Fish& f : this->allFish){
    //     if(fish.fishID != f.fishID){
    //         // note that in the following statement another condition is that the fish is not right behind the one we are currently observing
    //         if(glm::length(fish.joints[0].Center - f.joints[0].Center) < separationSight && glm::dot(resultDir, f.joints[0].Center - fish.joints[0].Center) > -0.7){
    //             separationVec += fish.joints[0].Center - f.joints[0].Center;
    //             alignmentVec  += f.joints[0].moveDirection;
    //             cohesionVec   += f.joints[0].Center;
    //             separationCounter++;
    //             counter++;
    //         }
    //         else if(glm::length(fish.joints[0].Center - f.joints[0].Center) < otherSight && glm::dot(resultDir, f.joints[0].Center - fish.joints[0].Center) > -0.7){
    //             alignmentVec  += f.joints[0].moveDirection;
    //             cohesionVec   += f.joints[0].Center;
    //             counter++;
    //         }
    //     }
    // }

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            for(int k = 0; k < this->boxes[fish.box_coords.x+i][fish.box_coords.y+j].num_of_boxed_fish; k++){
                const Fish& f = this->allFish.at(this->boxes[fish.box_coords.x+i][fish.box_coords.y+j].fish_indexes[k]);
                if(fish.fishID != f.fishID){
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

        }
    }   

    // glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    // this->hit_check_result = (hit_check_struct*) glMapNamedBuffer(this->ssbo, GL_READ_ONLY);
    // resultDir = Global::rotateVector(resultDir, this->hit_check_result[fish.fishID].result * edgeEvasionIntensity * delta_time);
    // // std::cout << hit_check_result[fish.fishID].result << std::endl;
    // // if(this->hit_check_result[fish.fishID].result)
    //     // std::cout << fish.fishID << std::endl;
    //
    // glUnmapNamedBuffer(this->ssbo);
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



