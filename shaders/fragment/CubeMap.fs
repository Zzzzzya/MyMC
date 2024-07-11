#version 330 core

in vec3 TexCoords;
flat in int index;

out vec4 FragColor;

#define MAX_TEXTURES 30
uniform samplerCube tex[MAX_TEXTURES];

void main() {
    FragColor = texture(tex[index], TexCoords);
}