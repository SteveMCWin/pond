#ifndef FISH_HANDLER_H
#define FISH_HANDLER_H

#include <iostream>
#include <vector>

#include "fish.h"
#include "shader.h"
#include "compute_shader.h"
#include "fishRenderer.h"

typedef struct {
    int* fish_indexes;
    int  num_of_boxed_fish;
} box_struct;

typedef struct {
    glm::vec2 move_dir;
    glm::vec2 cetner;
    int result;
    int padding;
} hit_check_struct;

class FishHandler{

public:

    std::vector<Fish> allFish;

    FishHandler();
    ~FishHandler();

    void addFish(Fish& fish);
    void addFish(glm::vec2* centers, float* distances, float* radii, int id, float speed = 15.0f);

    void boxTheFish();
    void resetBoxSizes();
    void calcFishHitChecks();
    glm::vec2 calcFishMoveDir(Fish& fish, float delta_time);    // responsible for boid-like behaviour of fish
    // should change the calcFushMoveDir to return void and at the end of the function just call the fish's updateMoveDir function

private:
    
    // box_struct boxes[static_cast<unsigned int>(Global::screenHalfSize*Global::aspectRatio)]
    //                 [static_cast<unsigned int>(Global::screenHalfSize*Global::aspectRatio/Global::fishSightRange)];
    box_struct** boxes;
    ComputeShader fishComputeShader;
    unsigned int ssbo;
    hit_check_struct hit_check_data[Global::numberOfFish];
    hit_check_struct* hit_check_result;
    float cohesionIntensity = 0.5f;
    float alignmentIntensity = 180.0f;
    float separationIntensity = 180.0f;
    float edgeEvasionIntensity = 80.0f;
};

#endif // !FISH_HANDLER_H
