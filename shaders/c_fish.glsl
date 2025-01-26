#version 460 core

uniform float screen_half_size;
uniform float aspect_ratio;
uniform float hit_check_distance;
uniform float degree_change;

const uint num_of_hit_checks = 13;

layout (local_size_x = num_of_hit_checks, local_size_y = 1, local_size_z = 1) in;

struct FishData {
    vec2 move_dir;
    vec2 center;
    int result;
};

layout(std430, binding=0) buffer HitCheckBuffer {
    FishData data[];
} hit_check_buff;

vec2 rotate(vec2 v, float angle) {
    // Create the 2D rotation matrix
    mat2 rotationMatrix = mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );

    // Multiply the vector by the rotation matrix
    return rotationMatrix * v;
}

void main(){

    int i = -int((gl_LocalInvocationID.x/2));

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
            // hit_check_buff.data[gl_WorkGroupID.x].result += -1;
            atomicAdd(hit_check_buff.data[gl_WorkGroupID.x].result, -1);
        }
        else{
            // hit_check_buff.data[gl_WorkGroupID.x].result +=  1;
            atomicAdd(hit_check_buff.data[gl_WorkGroupID.x].result,  1);
        }
        memoryBarrierBuffer();
        // barrier();
    }

}
