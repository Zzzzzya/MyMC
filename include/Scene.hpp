#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

#include "Header.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Player.hpp"
#include <GLFW/glfw3.h>

using Camera::BACKWARD;
using Camera::FORWARD;
using Camera::LEFT;
using Camera::RIGHT;

/* 场景类 */
/* 世界类 一切参数在此设定 */
class Scene {
  public:
    /* 视口大小 */
    int imageWidth = 1600;
    int imageHeight = 900;

    /* 时间 */
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    float curTime = 0.0f;

    /* 光标位置 */
    float LastCursorX = 0;
    float LastCursorY = 0;
    /* 鼠标是否第一次进入窗口 */
    bool firstMouse = true;
    /* 鼠标是否在窗口内 */
    bool cursorInWindow = true;

    /* 窗口 */
    GLFWwindow *window = nullptr;

    /* 模型 */
    vector<vector<vector<shared_ptr<Mesh>>>> Map;
    vector<shared_ptr<Shader>> Shaders;

    /* 玩家 */
    shared_ptr<Player> player = make_shared<Player>(vec3(0.0f, 0.0f, 5.0f));

    /* 地图 */
    int mapX = 10;
    int mapY = 10;
    int mapZ = 10;
    int CubeSize = 2.0f;

    Scene();
    void InitScene(void (*cursorPosCallback)(GLFWwindow *, double, double) = nullptr,
                   void (*scrollCallback)(GLFWwindow *, double, double) = nullptr);
    void MainLoop();
    void DestroyScene();

  private:
    int InitWindow(void (*cursorPosCallback)(GLFWwindow *, double, double) = nullptr,
                   void (*scrollCallback)(GLFWwindow *, double, double) = nullptr);
    int InitRender();
    int InitShaders();
    int InitTextures();
    int InitMap();

    void ProcessKeyInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (cursorInWindow) {
                cout << "ESC Pressed" << endl;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursorInWindow = false;
                firstMouse = true;
            }
        }
        if (!cursorInWindow)
            return;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player->camera.ProcessKeyBoard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player->camera.ProcessKeyBoard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player->camera.ProcessKeyBoard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player->camera.ProcessKeyBoard(RIGHT, deltaTime);
        }
    };
};

#endif