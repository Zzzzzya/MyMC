#version 450 core

in vec3 TexCoordsCubeMap;
in vec2 TexCoords;
flat in int index;
flat in int meshType;
in vec3 FragPos;

out vec4 FragColor;

uniform samplerCube tex;

void main() {
    vec4 Color = vec4(texture(tex, TexCoordsCubeMap).rgb, 1.0f);
    FragColor = Color;
}