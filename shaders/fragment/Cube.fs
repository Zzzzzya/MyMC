#version 330 core

in vec2 TexCoords;
flat in int index;

out vec4 FragColor;

#define MAX_TEXTURES 1024
uniform sampler2D tex[1024];

void main() {
    FragColor = (texture(tex[index], TexCoords).rgb, 0.0f);
}