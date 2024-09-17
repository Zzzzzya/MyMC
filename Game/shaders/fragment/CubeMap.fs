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

// shadow map
uniform sampler2D shadowMap;
uniform mat4 lightMatrix;
uniform float shadowBias;

float ShadowCalculate(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth - shadowBias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    // Color
    vec4 Color = meshType == 0 ? texture(tex[index], TexCoordsCubeMap) : texture(tex2D[index], TexCoords);
    if (Color.a < 0.1)
        discard;

    // Fog
    float distance = length(cameraPos - FragPos);
    float fogFactor = exp(-pow(fogDensity * distance, 2.0));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 fogColor = mix(fogColor, Color.rgb, fogFactor);

    // Shadow
    vec4 fragPosLightSpace = lightMatrix * vec4(FragPos, 1.0);
    float shadow = ShadowCalculate(fragPosLightSpace);
    vec3 shadowColor = fogColor * (1.0 - shadow * 0.2);

    // Final Color
    vec3 finalColor = shadowColor;
    FragColor = vec4(finalColor, Color.a);
}