#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_cords;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

void main(){
    gl_Position = u_projection*u_view*u_model*vec4(in_position, 1.0);
    frag_pos = vec3(u_model*vec4(in_position, 1.0));
    normal = mat3(transpose(inverse(u_model)))*in_normal; // normal matrix
    tex_cords = in_tex_coord;
}