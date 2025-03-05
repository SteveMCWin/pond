#version 450 core
precision highp float;

in  vec2 texCoords;

out vec4 outputColor;

uniform sampler2D fishTexture;

void main(){
    // outputColor = vec4(0.2, 0.5, 0.7, 1.0);
    vec4 texColor = texture(fishTexture, texCoords);
    if(texColor.b > 0.5) texColor = vec4(1.0);
    outputColor = texColor;
}
