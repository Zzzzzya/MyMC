#version 450 core
layout(location = 0) in vec2 aPos;

out vec2 TexCoords;

void main() {
    TexCoords = aPos.xy;
    gl_Position = vec4(aPos.xy, 1.0, 1.0);
}