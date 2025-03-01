#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D waterNoiseTexture;
uniform sampler2D highlightNoiseTexture;

uniform float iTime;


vec4 return_pixelated(float pixels){
    float delta = 1.0f/pixels;
    vec2 newCoords = vec2(delta * floor(TexCoords.x / delta), delta * floor(TexCoords.y / delta));
    return texture(screenTexture, newCoords);
}

vec4 return_normal(){
    return texture(screenTexture, TexCoords);
}

void main()
{

    FragColor = return_pixelated(256.0);


    // FragColor = return_normal();
}

