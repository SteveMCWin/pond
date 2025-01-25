#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding=2) buffer TestBuffer {
    float nums[10];
} test_buffer;

void main(){
    test_buffer.nums[gl_WorkGroupID.x] = test_buffer.nums[gl_WorkGroupID.x] * 2;
}
