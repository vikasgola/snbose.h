#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coord;

out vec2 texCoord;

uniform mat4 u_trs;

void main(){
    gl_Position = u_trs*vec4(in_position, 0.0, 1.0);
    texCoord = in_tex_coord;
}