#version 330 core
layout (location = 0) in vec3 vboPos;
layout (location = 1) in vec2 lCoords;

uniform float screen_resolution;
uniform vec2 center;

out vec2 localCoords;

void main(){
    gl_Position = vec4(vboPos.x*screen_resolution + center.x, vboPos.y + center.y, vboPos.z, 1.0);
    localCoords = lCoords;
}
