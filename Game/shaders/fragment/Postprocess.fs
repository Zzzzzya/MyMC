#version 450 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform sampler2D scene;
uniform sampler2D depthMap;
uniform float inWater;

float LinearizeDepth(float depth) {
    float near_plane = 0.1;
    float far_plane = 500.0;
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
    // FragColor = mix(vec4(0.3f, 0.4f, 0.9f, 0.5f), texture(scene, TexCoord), inWater);
    FragColor = texture(scene, TexCoord) * texture(screenTexture, TexCoord);
}