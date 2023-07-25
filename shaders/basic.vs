#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coord;

out vec2 texCoord;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);

void main(){
    gl_Position = u_view*u_model*vec4(in_position, 0.0, 1.0);
    texCoord = in_tex_coord;
}