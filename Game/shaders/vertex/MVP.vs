#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in int aFaceID;
layout(location = 3) in int aCubeID;

out vec2 TexCoords;
flat out int index;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0f);
    index = (aCubeID - 1) * 6 + aFaceID;
    TexCoords = aTexCoords;
}