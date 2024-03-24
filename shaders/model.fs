#version 330 core

layout(location = 0) out vec4 color;

in vec2 texCoord;
uniform sampler2D u_texture_Diffuse1;

void main(){
    color = texture(u_texture_Diffuse1, texCoord);
}