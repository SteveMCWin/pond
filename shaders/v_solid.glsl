#version 330 core
layout (location = 0) in vec2 vboPos;

uniform mat4 projection;
uniform mat4 model;

// out vec2 headPos;

void main(){
    vec4 pos = projection * model * vec4(vboPos, 0.0, 1.0);
    gl_Position = pos;
    // headPos = vboPos;
}
