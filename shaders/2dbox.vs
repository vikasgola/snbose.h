#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec2 texCoord;

uniform mat4 u_model = mat4(1.0);

void main(){
    gl_Position = u_model*vec4(in_position, 1.0);
    texCoord = in_tex_coord;
}