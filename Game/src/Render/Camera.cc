#include "Camera.hpp"
#include "Map.hpp"

Camera::Camera(const vec3 &pos) : position(pos) {
    updateCameraVectors();
}

mat4 Camera::ViewMat() const {
    return glm::lookAt(position, position + front, up);
}

mat4 Camera::ViewBackMat() const {
    return glm::lookAt(position, position - front, up);
}

void Camera::ProcessKeyBoard(Movement move, float deltaTime, shared_ptr<Map> map) {
    vec3 movVec;
    float movDis = deltaTime * moveSpeed;
    vec3 N = vec3(0.0f, 1.0f, 0.0f);
    vec3 trueFront = GameMode_Survival ? normalize(front - dot(front, N) * N) : front;
    vec3 trueRight = GameMode_Survival ? normalize(cross(trueFront, N)) : right;
    vec3 offset;
    switch (move) {
    case FORWARD:
        movVec = trueFront * movDis;
        offset = trueFront;
        break;
    case BACKWARD:
        movVec = trueFront * (-movDis);
        offset = -trueFront;
        break;
    case LEFT:
        movVec = -trueRight * movDis;
        offset = -trueRight;
        break;
    case RIGHT:
        movVec = trueRight * (movDis);
        offset = trueRight;
        break;
    default:
        movVec = N * (2 * movDis);
        offset = N;
        break;
    }

    if (map->CheckCollision(position + 2.0f * offset, movVec) && map->CheckCollision(position + 1.2f * offset, movVec))
        position += movVec;
}

void Camera::ProcessCursorPos(float offsetX, float offsetY) {
    yaw += offsetX * turnSpeed;
    pitch += offsetY * turnSpeed;

    pitch = std::fmin(std::fmax(pitch, -89.0f), 89.0f);

    updateCameraVectors();
}

void Camera::ProcessScroll(float offsetY) {
    fov -= offsetY;
    fov = std::fmin(fov, 90.0f);
    fov = std::fmax(fov, 1.0f);
}

void Camera::ProcessDropDown(float deltaTime, const shared_ptr<Map> &map) {
    vec3 movVec = vec3(0.0f, -1.0f, 0.0f) * moveSpeed * deltaTime;
    if (map->CheckCollisionSingle(position + vec3(0.0f, -3.0f, 0.0f) + movVec) &&
        map->CheckCollisionSingle(position + vec3(0.0f, -2.0f, 0.0f) + movVec) &&
        map->CheckCollisionSingle(position + vec3(0.0f, -1.0f, 0.0f) + movVec))
        position += movVec;
}

void Camera::ProcessJump(float deltaTime, const shared_ptr<Map> &map) {
    vec3 movVec = vec3(0.0f, 1.0f, 0.0f) * moveSpeed * deltaTime * 3.0f;
    if (map->CheckCollisionSingle(position + vec3(0.0f, 3.0f, 0.0f) + movVec) &&
        map->CheckCollisionSingle(position + vec3(0.0f, 2.0f, 0.0f) + movVec) &&
        map->CheckCollisionSingle(position + vec3(0.0f, 1.0f, 0.0f) + movVec))
        position += movVec;
}

void Camera::updateCameraVectors() {
    front.y = sin(radians(pitch));
    front.x = cos(radians(pitch)) * cos(radians(yaw));
    front.z = cos(radians(pitch)) * sin(radians(yaw));

    front = normalize(front);
    right = normalize(glm::cross(front, vec3(0.0f, 1.0f, 0.0f)));
    up = normalize(glm::cross(right, front));
}
