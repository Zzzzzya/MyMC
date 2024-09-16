#version 450 core

in vec2 TexCoords;

out vec4 SkyColor;

void main() {
    vec4 BlueSky = vec4(0.53, 0.81, 0.98, 1.0);
    SkyColor = mix(BlueSky, vec4(1.0, 1.0, 1.0, 1.0), 1 - TexCoords.y);
}