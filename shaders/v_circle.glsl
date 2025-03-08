#version 450 core
layout (location = 0) in vec2 vboPos;

uniform float r;

uniform mat4 model;
uniform mat4 projection;

out vec2 localCoords;
out float radius;

void main(){
    // gl_Position = vec4(vboPos.x*aspect_ratio*r + center.x, vboPos.y*r + center.y, 0.0, 1.0);
    gl_Position = projection * model * vec4(vboPos, 0.0, 1.0);
    localCoords = vboPos*r;
    radius = r;
}
