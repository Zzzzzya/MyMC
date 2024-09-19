#pragma once
#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include "Header.hpp"
#include "AABB.hpp"
#include "Camera.hpp"

struct Plane {
    vec3 normal;    // 平面法向量
    float distance; // 平面与原点的距离

    // 点到平面的距离
    float distanceToPoint(const glm::vec3 &point) const {
        return glm::dot(normal, point) + distance;
    }

    void normalize() {
        auto len = glm::length(normal);
        normal /= len;
        distance /= len;
    }
};

struct Frustum {
    Frustum() = default;
    Frustum(const Camera &, float);
    Plane planes[6]; // 视锥体的六个平面
    bool isAABBInFrustum(const AABB &box);

  private:
    void extractPlanes(const glm::mat4 &vpMatrix);
    // 找到 AABB 在平面法线方向上的最远点
    glm::vec3 getFarthestPoint(const AABB &aabb, const glm::vec3 &normal) const;

    // 找到 AABB 在平面法线方向上的最近点
    glm::vec3 getNearestPoint(const AABB &aabb, const glm::vec3 &normal) const;
};

// 判断 AABB 是否与视锥体相交

#endif