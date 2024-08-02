#version 330 core
precision highp float;

in vec2 localCoords;
in float radius;
out vec4 outputColor;

void main(){
    float line_width = 0.05;
    vec4 color = vec4(step(radius - line_width, length(localCoords)) * (1 - step(radius, length(localCoords))));

    outputColor = color;
}
