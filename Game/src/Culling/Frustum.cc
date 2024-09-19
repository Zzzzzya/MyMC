#include "Frustum.hpp"

Frustum::Frustum(const Camera &camera, float aspect) {
    // 获取相机的视图矩阵和投影矩阵
    glm::mat4 viewMatrix = camera.ViewMat();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), aspect, 0.1f, 1000.0f);

    // 计算视图投影矩阵
    glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    // 提取视锥体的平面（近平面、远平面、左、右、上、下）
    extractPlanes(viewProjectionMatrix);
}

void Frustum::extractPlanes(const glm::mat4 &vpMatrix) {
    // 近平面: M[3] + M[2]
    planes[0].normal =
        glm::vec3(vpMatrix[0][3] + vpMatrix[0][2], vpMatrix[1][3] + vpMatrix[1][2], vpMatrix[2][3] + vpMatrix[2][2]);
    planes[0].distance = vpMatrix[3][3] + vpMatrix[3][2];
    planes[0].normalize();

    // 远平面: M[3] - M[2]
    planes[1].normal =
        glm::vec3(vpMatrix[0][3] - vpMatrix[0][2], vpMatrix[1][3] - vpMatrix[1][2], vpMatrix[2][3] - vpMatrix[2][2]);
    planes[1].distance = vpMatrix[3][3] - vpMatrix[3][2];
    planes[1].normalize();

    // 左平面: M[3] + M[0]
    planes[2].normal =
        glm::vec3(vpMatrix[0][3] + vpMatrix[0][0], vpMatrix[1][3] + vpMatrix[1][0], vpMatrix[2][3] + vpMatrix[2][0]);
    planes[2].distance = vpMatrix[3][3] + vpMatrix[3][0];
    planes[2].normalize();

    // 右平面: M[3] - M[0]
    planes[3].normal =
        glm::vec3(vpMatrix[0][3] - vpMatrix[0][0], vpMatrix[1][3] - vpMatrix[1][0], vpMatrix[2][3] - vpMatrix[2][0]);
    planes[3].distance = vpMatrix[3][3] - vpMatrix[3][0];
    planes[3].normalize();

    // 上平面: M[3] - M[1]
    planes[4].normal =
        glm::vec3(vpMatrix[0][3] - vpMatrix[0][1], vpMatrix[1][3] - vpMatrix[1][1], vpMatrix[2][3] - vpMatrix[2][1]);
    planes[4].distance = vpMatrix[3][3] - vpMatrix[3][1];
    planes[4].normalize();

    // 下平面: M[3] + M[1]
    planes[5].normal =
        glm::vec3(vpMatrix[0][3] + vpMatrix[0][1], vpMatrix[1][3] + vpMatrix[1][1], vpMatrix[2][3] + vpMatrix[2][1]);
    planes[5].distance = vpMatrix[3][3] + vpMatrix[3][1];
    planes[5].normalize();
}

glm::vec3 Frustum::getFarthestPoint(const AABB &aabb, const glm::vec3 &normal) const {
    glm::vec3 point = aabb.min;

    // 根据法线方向选择 AABB 的最远点
    if (normal.x >= 0)
        point.x = aabb.max.x;
    if (normal.y >= 0)
        point.y = aabb.max.y;
    if (normal.z >= 0)
        point.z = aabb.max.z;

    return point;
}

glm::vec3 Frustum::getNearestPoint(const AABB &aabb, const glm::vec3 &normal) const {
    glm::vec3 point = aabb.max;

    // 根据法线方向选择 AABB 的最近点
    if (normal.x >= 0)
        point.x = aabb.min.x;
    if (normal.y >= 0)
        point.y = aabb.min.y;
    if (normal.z >= 0)
        point.z = aabb.min.z;

    return point;
}

bool Frustum::isAABBInFrustum(const AABB &aabb) {
    // 遍历视锥体的六个平面
    for (const auto &plane : planes) {
        // 找到 AABB 在平面法线方向上的最远点和最近点
        glm::vec3 farthestPoint = getFarthestPoint(aabb, plane.normal);
        glm::vec3 nearestPoint = getNearestPoint(aabb, plane.normal);

        // 计算最近点和最远点到平面的距离
        float farthestDistance = plane.distanceToPoint(farthestPoint);
        float nearestDistance = plane.distanceToPoint(nearestPoint);

        // 如果最近点和最远点都在平面的负侧，AABB 完全在平面外，返回 false
        if (farthestDistance < 0 && nearestDistance < 0) {
            return false; // AABB 完全位于平面外
        }
    }

    // AABB 与视锥体相交或完全在视锥体内
    return true;
}
