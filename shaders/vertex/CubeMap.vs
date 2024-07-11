#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in int aFaceID;
layout(location = 3) in int aCubeID;
layout(location = 4) in vec3 aTexCoordsCubeMap;

out vec3 TexCoordsCubeMap;
out vec2 TexCoords;
flat out int index;
flat out int meshType;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0f);
    index = aCubeID - 1;
    meshType = aFaceID;
    TexCoordsCubeMap = aTexCoordsCubeMap;
    TexCoords = aTexCoords;
}