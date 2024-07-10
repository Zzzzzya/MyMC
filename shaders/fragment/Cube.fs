#version 330 core

in vec2 TexCoords;
flat in int index;

out vec4 FragColor;

uniform sampler2D tex[30];

const int CubeCount = 6;
vec3 colors[CubeCount] = vec3[](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 0.0),
                                vec3(1.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0));

void main() {

    FragColor = texture(tex[index], TexCoords);
}