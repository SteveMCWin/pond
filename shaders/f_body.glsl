#version 450 core
precision highp float;

in  vec2 texCoords;

out vec4 outputColor;

uniform sampler2D fishTexture;

uniform int use_solid_color;
uniform vec3 body_color;

void main(){
    // outputColor = vec4(0.2, 0.5, 0.7, 1.0);
    vec4 texColor = texture(fishTexture, texCoords);
    outputColor = texColor * (1-use_solid_color) + vec4(body_color, 1.0) * use_solid_color;
}
