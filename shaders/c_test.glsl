#version 460 core

layout (local_size_x = 13, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding=2) buffer TestBuffer {
    int num[];
} test_buffer;

void main(){
    atomicAdd(test_buffer.num[gl_WorkGroupID.x], int(gl_WorkGroupID.x));
}
