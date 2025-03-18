#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int use_pixelation;
uniform float pixelation_amount;

vec4 return_pixelated(){
    float delta = 1.0f/pixelation_amount;
    vec2 newCoords = vec2(delta * floor(TexCoords.x / delta), delta * floor(TexCoords.y / delta));
    return texture(screenTexture, newCoords);
}

vec4 return_normal(){
    return texture(screenTexture, TexCoords);
}

void main() {
    FragColor = return_normal() * (1-use_pixelation) + return_pixelated() * (use_pixelation);
}

