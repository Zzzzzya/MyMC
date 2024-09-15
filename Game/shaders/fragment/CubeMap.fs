#version 450 core

in vec3 TexCoordsCubeMap;
in vec2 TexCoords;
flat in int index;
flat in int meshType;
in vec3 FragPos;

out vec4 FragColor;

#define MAX_TEXTURES 16
uniform sampler2D tex2D[MAX_TEXTURES];
uniform samplerCube tex[MAX_TEXTURES];

uniform vec3 fogColor;    // 雾的颜色
uniform float fogDensity; // 雾的密度
uniform float fogStart;   // 线性雾开始的距离
uniform float fogEnd;     // 线性雾结束的距离

uniform vec3 cameraPos;

void main() {
    vec4 Color = meshType == 0 ? texture(tex[index], TexCoordsCubeMap) : texture(tex2D[index], TexCoords);
    if (Color.a < 0.1)
        discard;

    float distance = length(cameraPos - FragPos);
    float fogFactor = exp(-pow(fogDensity * distance, 2.0));
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    FragColor = vec4(mix(fogColor, vec3(Color), fogFactor), Color.a);
}