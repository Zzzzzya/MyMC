#pragma once
#ifndef AABB_HPP
#define AABB_HPP

#include "Header.hpp"

struct AABB {
    AABB() = default;
    AABB(const glm::vec3 &v1, const glm::vec3 &v2) {
        min.x = std::min(v1.x, v2.x);
        min.y = std::min(v1.y, v2.y);
        min.z = std::min(v1.z, v2.z);
        max.x = std::max(v1.x, v2.x);
        max.y = std::max(v1.y, v2.y);
        max.z = std::max(v1.z, v2.z);
    }
    vec3 min; // AABB 的最小点
    vec3 max; // AABB 的最大点
};

#endif