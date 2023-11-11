#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec2 texCoord;

void main(){
    gl_Position = vec4(in_position, 1.0);
    texCoord = in_tex_coord;
}