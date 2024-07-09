#pragma once

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Header.hpp"

class Camera {
  public:
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    vec3 front = vec3(0.0f, 0.0f, -1.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = vec3(1.0f, 0.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float fov = 45.0f;

    float moveSpeed = 10.0f;
    float turnSpeed = 0.05f;

    enum Movement {
        FORWARD = 0,
        BACKWARD,
        LEFT,
        RIGHT
    };

  public:
    Camera(const vec3 &pos);
    mat4 ViewMat() const;
    mat4 ViewBackMat() const;
    void ProcessKeyBoard(Movement move, float deltaTime);
    void ProcessCursorPos(float offsetX, float offsetY);
    void ProcessScroll(float offsetY);

  private:
    void updateCameraVectors();
};
#endif