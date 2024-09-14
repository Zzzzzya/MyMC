#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D spark;

void main() {
    FragColor = texture(spark, TexCoords);
    if (FragColor.a < 0.1) {
        discard;
    }
}