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
}hit_check_buff;

vec2 rotate(vec2 v, float angle) {
    // Create the 2D rotation matrix
    mat2 rotationMatrix = mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );

    // Multiply the vector by the rotation matrix
    return rotationMatrix * v;
}

shared int res = 0;

void main(){

    int i = -int((gl_LocalInvocationID.x/2));

    vec2 hit_check_local_pos = rotate(hit_check_buff.data[gl_WorkGroupID.x].move_dir, -degree_change*i);
    vec2 hit_check_world_pos = hit_check_buff.data[gl_WorkGroupID.x].center + hit_check_local_pos * hit_check_distance;

    if(pow(abs(hit_check_world_pos.x/screen_half_size), 4) + pow(abs(hit_check_world_pos.y/(screen_half_size*aspect_ratio)), 4) >= 0.8){
        if(i < 0){
            atomicAdd(hit_check_buff.data[gl_WorkGroupID.x].result, -1);
        }
        else{
            atomicAdd(hit_check_buff.data[gl_WorkGroupID.x].result,  1);
        }
    }

    // atomicAdd(hit_check_buff.data[1].result,  10);

}
