#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;

out vec2 texCoords;

void main(){
    vec4 pos = projection * model * vec4(aPos, 0.0, 1.0);
    gl_Position = pos;
    texCoords = TexCoords;
}
