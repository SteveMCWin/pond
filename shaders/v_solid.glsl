#version 330 core
layout (location = 0) in vec2 vboPos;

uniform mat4 projection;
uniform mat4 model;

void main(){
    // gl_Position = vec4(vboPos.x*aspect_ratio*r + center.x, vboPos.y*r + center.y, 0.0, 1.0);
    gl_Position = projection * model * vec4(vboPos, 0.0, 1.0);
}
