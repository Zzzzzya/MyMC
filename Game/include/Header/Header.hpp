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
using glm::vec4;

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

inline float distance(const vec3 &a, const vec3 &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

inline float distance2(const vec3 &a, const vec3 &b) {
    return pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2);
}

inline int getCPUCores() {
#if defined(_WIN32)
// 命令调取 Cpu 物理核数
#define CpuCoreNum "wmic cpu get NumberOfCores"

    FILE *fp = _popen(CpuCoreNum, "r");
    if (fp == nullptr) {
        return 0;
    }
    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);
    _pclose(fp);
    int coreNum = atoi(buffer);
    return coreNum;
#elif defined(LINUX) || defined(SOLARIS) || defined(AIX)
    return get_nprocs();   //GNU fuction
#else
#error  UNKNOWN OS!
#endif
}

#include <thread>

#include "Log/Logger.hpp"
#endif