#version 460 core

layout (local_size_x = num_of_hit_checks, local_size_y = 1, local_size_z = 1) in;

uniform float screen_half_size;
uniform float hit_check_distance;
uniform float degree_change;
uniform uint num_of_hit_checks;

struct FishData {
    vec2 move_dir;
    vec2 center;
    int result[num_of_hit_checks];
};

layout(std430, binding=0) buffer HitCheckBuffer {
    FishData data[];
} hit_check_buff;

void main(){
    // test_buffer.nums[gl_WorkGroupID.x] = test_buffer.nums[gl_WorkGroupID.x] * 2;
    hit_check_buff.data[gl_WorkGroupID].result[gl_LocalInvocationID] = 0;

    int i = -(gl_LocalInvocationID.x/2);

    /*
        this->hit_checks[j] = Global::rotateVector(this->joints[0].moveDirection, -degreeChange * i);
        
        glm::vec2 hit_check_world_pos = this->joints[0].Center + this->hit_checks[j] * hit_check_distance;

        // The if checks if the position of the hit check is outside a square with round corners, the exponent (4 in this case) determines the roundness (smaller exp -> rounder)
        // the result of 1.0 is just on the edge of the window, less than is inside, more than is on the outside
        if( powf(std::abs(hit_check_world_pos.x/Global::screenHalfSize), 4) +
            powf(std::abs(hit_check_world_pos.y/(Global::screenHalfSize*Global::aspectRatio)), 4) >= 1.0){

                hit_checks_result += (i < 0) ? -1 : 1;
        }
    */

    // WE ARE ASSUMING THAT DEGREE_CHANGE IS IN RADIANS
    vec2 hit_check_local_pos = rotate(hit_check_buff.data[gl_WorkGroupID.x].move_dir, degree_change);
    vec2 hit_check_world_pos = hit_check_buff.data[gl_WorkGroupID.x].center + hit_check_local_pos * hit_check_distance;

    if(pow(abs(hit_check_world_pos.x/screen_half_size), 4) + pow(abs(hit_check_world_pos.y/screen_half_size*aspect_ratio), 4) >= 1.0){
        if(i < 0){
            hit_check_buff.data[gl_WorkGroupID.x].result[gl_LocalInvocationID.x] = -1;
        }
        else{
            hit_check_buff.data[gl_WorkGroupID.x].result[gl_LocalInvocationID.x] =  1;
        }
    }



}
