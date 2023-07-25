#version 330 core

layout(location = 0) out vec4 color;

in vec2 texCoord;

uniform vec4 u_color = vec4(1.0);
uniform sampler2D texture1;

void main(){
    color = texture(texture1, texCoord)*u_color;
}