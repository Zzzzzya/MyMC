#version 450 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform sampler2D scene;
uniform sampler2D depthMap;
uniform samplerCube cubeMap;
uniform float inWater;
uniform int hasBlock;

float LinearizeDepth(float depth) {
    float near_plane = 0.1;
    float far_plane = 500.0;
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
    // FragColor = mix(vec4(0.3f, 0.4f, 0.9f, 0.5f), texture(scene, TexCoord), inWater);

    // 拿到屏幕坐标
    vec2 screenCoords = TexCoord;
    screenCoords.y *= 900.0;
    screenCoords.x *= 1600;

    FragColor = texture(scene, TexCoord) * texture(screenTexture, TexCoord);

    if (hasBlock == 1) {
        int sx = 45;
        int sy = 45;
        float x = screenCoords.x;
        float y = screenCoords.y;
        vec3 a = vec3((x - sx) * 2 / 150 - 1, (y - sy) * 2 / 150 - 1, 1);
        vec3 p = vec3(0.0f, 0.0f, 0.0f);
        vec3 cai = normalize(a - p);
        if (screenCoords.y >= sy && screenCoords.y <= sy + 150 && screenCoords.x >= sx && screenCoords.x <= sx + 150) {
            FragColor = texture(cubeMap, cai);
        }
    }
}