#pragma once
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Header.hpp"
#include "Camera.hpp"

class Player {
  public:
    Player(const vec3 &pos = vec3(0.0f, 0.0f, 0.0f));

    Camera camera;

    bool inWater = false;
    vec3 curBlockPos;
};

#endif