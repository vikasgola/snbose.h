#version 330 core

layout(location = 0) out vec4 color;

in vec2 texCoord;

uniform vec3 u_color = vec3(1.0);
uniform sampler2D u_texture1;

void main(){
    color = texture(u_texture1, texCoord)*vec4(u_color, 1.0);
}