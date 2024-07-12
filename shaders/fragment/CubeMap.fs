#version 450 core

in vec3 TexCoordsCubeMap;
in vec2 TexCoords;
flat in int index;
flat in int meshType;

out vec4 FragColor;

#define MAX_TEXTURES 20
uniform sampler2D tex2D[MAX_TEXTURES];
uniform samplerCube tex[MAX_TEXTURES];

void main() {
    FragColor = meshType == 0 ? texture(tex[index], TexCoordsCubeMap) : texture(tex2D[index], TexCoords);
    if (FragColor.a < 0.1)
        discard;
}