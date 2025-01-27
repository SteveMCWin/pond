#version 460 core

layout (local_size_x = 13, local_size_y = 1, local_size_z = 1) in;

struct TestStruct {
    vec2 some_vec;
    int res;
};

layout(std430, binding=2) buffer TestBuffer {
    TestStruct data[];
} test_buffer;

void main(){
    atomicAdd(test_buffer.data[gl_WorkGroupID.x].res, 1);
}
