#version 450 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float inWater;

void main() {
    FragColor = mix(vec4(0.3f, 0.4f, 0.9f, 0.5f), texture(screenTexture, TexCoord), inWater);
}