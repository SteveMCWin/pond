#version 450 core
precision highp float;

uniform vec3 Color;

out vec4 outputColor;

void main(){
    outputColor = vec4(Color, 1.0);
}
