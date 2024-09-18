#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 oNormal;

in vec3 TexCoordsCubeMap;
in vec2 TexCoords;
flat in int index;
flat in int meshType;
in vec3 FragPos;
in vec3 Normal;

#define MAX_TEXTURES 16
uniform sampler2D tex2D[MAX_TEXTURES];
uniform samplerCube tex[MAX_TEXTURES];

uniform vec3 fogColor;    // 雾的颜色
uniform float fogDensity; // 雾的密度
uniform float fogStart;   // 线性雾开始的距离
uniform float fogEnd;     // 线性雾结束的距离

uniform vec3 cameraPos;

// shadow map
uniform sampler2D shadowMap;
uniform mat4 lightMatrix;
uniform float shadowBias;

float ShadowCalculate(vec4 fragPosLightSpace) {
    float shadow = 0.0f;

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

    float texelSize = 1.0 / 2048;
    for (int x = -3; x <= 3; ++x) {
        for (int y = -3; y <= 3; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * vec2(texelSize, texelSize)).r;
            shadow += currentDepth - shadowBias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 49.0;
    return shadow;
}

void main() {
    // Color
    vec4 Color = vec4(38.0 / 255.0, 0.0 / 255.0, 191.0 / 255.0, 0.8f);
    // vec4 Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    // Shadow
    vec4 fragPosLightSpace = lightMatrix * vec4(FragPos, 1.0);
    float shadow = ShadowCalculate(fragPosLightSpace);
    vec3 shadowColor = Color.rgb * (1.0 - shadow * 0.5);

    // Fog
    float distance = length(cameraPos - FragPos);
    float fogFactor = exp(-pow(fogDensity * distance, 2.0));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 fogColor = mix(fogColor, shadowColor, fogFactor);

    // Final Color
    vec3 finalColor = fogColor;
    FragColor = Color;
    oNormal = Normal;
}