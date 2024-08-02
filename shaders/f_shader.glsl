#version 330 core
precision highp float;

in vec2 localCoords;
out vec4 outputColor;

void main(){
    vec4 color = vec4(step(0.95, length(localCoords)) * (1 - step(1.0, length(localCoords))));

    outputColor = color;
}
