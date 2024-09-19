#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 oNormal;

in vec3 TexCoordsCubeMap;
in vec2 TexCoords;
flat in int index;
flat in int meshType;
in vec3 FragPos;
in vec3 Normal;

uniform samplerCube tex;

void main() {
    vec4 Color = vec4(texture(tex, TexCoordsCubeMap).rgb, 1.0f);
    FragColor = Color;
    oNormal = Normal;
}