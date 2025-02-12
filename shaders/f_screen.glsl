#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D waterNoiseTexture;
uniform sampler2D highlightNoiseTexture;

uniform float iTime;

const float offset = 1.0 / 1000.0;

float avg(vec4 vec){
    return (vec.x + vec.y + vec.z) / 3.0;
}

void main()
{

/*

float avg(vec4 color) {
    return (color.r + color.g + color.b)/3.0;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Flow Speed, increase to make the water flow faster.
    float speed = 1.0;
    
    // Water Scale, scales the water, not the background.
    float scale = 0.8;
    
    // Water opacity, higher opacity means the water reflects more light.
    float opacity = 0.5;
 
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = (fragCoord/iResolution.xy);
    vec2 scaledUv = uv*scale;

    // Water layers, layered on top of eachother to produce the reflective effect
    // Add 0.1 to both uv vectors to avoid the layers stacking perfectly and creating a huge unnatural highlight
    vec4 water1 = texture(iChannel0, scaledUv/8.0 + iTime*0.02*speed - 0.1);
    vec4 water2 = texture(iChannel0, scaledUv.xy/2.0 + iTime*speed*vec2(-0.02, -0.02) + 0.1);
    
    // Water highlights
    vec4 highlights1 = texture(iChannel2, scaledUv.xy*4.0 + iTime*speed / vec2(-10, 100));
    vec4 highlights2 = texture(iChannel2, scaledUv.xy*4.0 + iTime*speed / vec2(10, 100));
    
    // Background image
    vec4 background = texture(iChannel1, vec2(uv) + avg(water1) * 0.05);
    
    // Average the colors of the water layers (convert from 1 channel to 4 channel
    water1.rgb = vec3(avg(water1));
    water2.rgb = vec3(avg(water2));
    
    // Average and smooth the colors of the highlight layers
    highlights1.rgb = vec3(avg(highlights1)/1.5);
    highlights2.rgb = vec3(avg(highlights2)/1.5);
    
    float alpha = opacity;
    
    if(avg(water1 + water2) > 0.3) {
        alpha = 0.0;
    }
    
    if(avg(water1 + water2 + highlights1 + highlights2) > 0.75) {
        alpha = 5.0 * opacity;
    }

    // Output to screen
    fragColor = (water1 + water2) * alpha + background;
}

*/

    // float speed = 1.0;
    // float scale = 0.2;
    // float opacity = 0.1;
    //
    // vec2 scaledTexCoords = TexCoords * scale;
    // // scaledTexCoords.x = scaledTexCoords.x * 16.0/3.0;
    // // scaledTexCoords.y = scaledTexCoords.y / (16.0/9.0);
    //
    // vec4 water1 = texture(waterNoiseTexture, scaledTexCoords + iTime*0.02 * speed - 0.1);
    // vec4 water2 = texture(waterNoiseTexture, scaledTexCoords + iTime*-0.02 * speed + 0.1);
    //
    // vec4 highlights1 = texture(highlightNoiseTexture, scaledTexCoords + iTime*speed / vec2(-10.0, 100.0));
    // vec4 highlights2 = texture(highlightNoiseTexture, scaledTexCoords + iTime*speed / vec2( 10.0, 100.0));
    //
    // vec4 background = texture(screenTexture, TexCoords + avg(water1) * 0.05);
    vec4 background = texture(screenTexture, TexCoords);
    //
    // water1.rgb = vec3(avg(water1));
    // water2.rgb = vec3(avg(water2));
    //
    // highlights1.rgb = vec3(avg(highlights1)/1.5);
    // highlights2.rgb = vec3(avg(highlights2)/1.5);
    //
    // float alpha = opacity;
    //
    // if(avg(water1+water2) > 0.3){
    //     alpha = 0.0;
    // }
    //
    // if(avg(water1 + water2 + highlights1 + highlights2) > 0.75){
    //     alpha = 4.0 * opacity;
    // }
    //
    // FragColor = (water1 + water2) * alpha + background;
    FragColor = background;
} 
