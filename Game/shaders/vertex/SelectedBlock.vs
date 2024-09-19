#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in int aFaceID;
layout(location = 3) in int aCubeID;
layout(location = 4) in vec3 aTexCoordsCubeMap;
layout(location = 5) in vec3 aNormal;

out vec3 TexCoordsCubeMap;
out vec2 TexCoords;
flat out int index;
flat out int meshType;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0f);
    index = aCubeID - 1;
    meshType = aFaceID;
    TexCoordsCubeMap = aTexCoordsCubeMap;
    TexCoords = aTexCoords;
    Normal = aNormal;
}