#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

#include "Header.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Player.hpp"
#include <GLFW/glfw3.h>

/* 场景类 */
/* 世界类 一切参数在此设定 */
class Scene {
  public:
    /* 世界参数 */
    int imageWidth = 1600;
    int imageHeight = 900;
    float curTime = 0.0f;
    float lastTime = 0.0f;
    float deltaTime = 0.0f;

    /* 窗口 */
    GLFWwindow *window = nullptr;

    /* 模型 */
    vector<shared_ptr<Mesh>> Meshes;
    vector<shared_ptr<Shader>> Shaders;

    /* 玩家 */
    shared_ptr<Player> player = make_shared<Player>(vec3(0.0f, 0.0f, 5.0f));

    Scene() = default;
};

#endif