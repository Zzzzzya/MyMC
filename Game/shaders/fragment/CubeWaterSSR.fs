#version 450 core
out vec4 FragColor;

in vec3 TexCoordsCubeMap;
in vec2 TexCoords;
flat in int index;
flat in int meshType;
in vec3 FragPos;
in vec3 Normal;
in mat4 cameraMatrix;

uniform vec3 cameraPos;

uniform sampler2D depth;
uniform sampler2D scene;

vec2 GetScreenTexCoord(vec3 worldPos) {
    vec4 clipSpace = cameraMatrix * vec4(worldPos, 1.0);
    vec3 ndc = clipSpace.xyz / clipSpace.w;
    return (ndc.xy + 1.0) / 2.0;
}

float GetDepth(vec3 worldPos) {
    float near_plane = 0.1;
    float far_plane = 500.0;
    vec4 clipSpace = cameraMatrix * vec4(worldPos, 1.0);
    vec3 ndc = clipSpace.xyz / clipSpace.w;
    float z = ndc.z;
    return z;
}

float GetGBufferDepth(vec2 uv) {
    float near_plane = 0.1;
    float far_plane = 500.0;
    float d = texture(depth, uv).r;
    float z = d * 2.0 - 1.0;
    return z;
}

bool RayTrace(vec3 origin, vec3 direction, float dis, out vec3 hitPoint) {
    const float step = 0.3;
    int stepNum = int(dis / step);
    float threshold = 0.00000001;
    vec3 cur = origin;
    vec3 stepVec = direction * step;

    for (int t = 0; t < 400; t += 1) {
        cur += stepVec;

        vec2 curTexCoord = GetScreenTexCoord(cur);
        if (curTexCoord.x < 0.0 || curTexCoord.x > 1.0 || curTexCoord.y < 0.0 || curTexCoord.y > 1.0) {
            break;
        }

        float curGBufferDepth = GetGBufferDepth(curTexCoord);
        float curDepth = GetDepth(cur);

        if (curDepth > curGBufferDepth + 0.0001) {
            break;
        }

        if (curDepth > curGBufferDepth + threshold) {
            hitPoint = cur;
            return true;
        }
    }
    return false;
}

void main() {
    vec4 Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    vec3 hitPoint;
    float dis = length(FragPos - cameraPos);
    if (dis > 500.0) {
        FragColor = Color;
        return;
    }

    vec3 ViewVec = normalize(FragPos - cameraPos);
    vec3 ReflectVec = normalize(reflect(ViewVec, normalize(Normal)));

    if (RayTrace(FragPos, ReflectVec, dis, hitPoint)) {
        Color = texture(scene, GetScreenTexCoord(hitPoint));
    }

    FragColor = Color; // Output the final color after SSR
}
