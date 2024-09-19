#version 450 core

in vec3 TexCoords;

out vec4 SkyColor;

uniform samplerCube Sky;

void main() {
    SkyColor = vec4(texture(Sky, TexCoords, 1.0).rgb, 1.0f);
}