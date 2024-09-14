#pragma once

#ifndef HEADER_HPP
#define HEADER_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

using glm::cross;
using glm::dot;

using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;

using glm::radians;

#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <unordered_map>
using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::to_string;
using std::unordered_map;
using std::vector;

#include <random>
inline float rand(float min, float max) {
    std::random_device rd;  // 用于获取随机数种子
    std::mt19937 gen(rd()); // 标准 mersenne_twister_engine
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

inline int rand(int min, int max) {
    std::random_device rd;  // 用于获取随机数种子
    std::mt19937 gen(rd()); // 标准 mersenne_twister_engine
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

#include <thread>

#include "Log/Logger.hpp"
#endif