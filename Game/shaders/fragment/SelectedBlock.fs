#version 450 core

out vec4 FragColor;

in vec3 TexCoordsCubeMap;
in vec2 TexCoords;
flat in int index;
flat in int meshType;
in vec3 FragPos;
in vec3 Normal;

uniform samplerCube tex;

void main() {
    // Color
    vec4 Color = texture(tex, TexCoordsCubeMap);
    if (Color.a < 0.1)
        discard;

    FragColor = vec4(1.0f);
}