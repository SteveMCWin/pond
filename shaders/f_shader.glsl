#version 330 core
precision highp float;

in vec2 localCoords;
in float radius;
out vec4 outputColor;

void main(){
    float line_width = 0.08;
    float inner_edge = smoothstep(radius - line_width * 1.5, radius - line_width, length(localCoords));
    float outer_edge = 1.0 - smoothstep(radius - line_width * 0.5, radius, length(localCoords));
    vec4 color = vec4(inner_edge * outer_edge);

    outputColor = color;
}
