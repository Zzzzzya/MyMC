#version 450 core

layout(location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 lightMatrix;

void main() {
    gl_Position = lightMatrix * vec4(aPos, 1.0f);
}