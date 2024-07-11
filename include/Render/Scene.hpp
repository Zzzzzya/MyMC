#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

#include "Header.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Player.hpp"

#include <GLFW/glfw3.h>

#include "Application.hpp"

using Camera::BACKWARD;
using Camera::FORWARD;
using Camera::LEFT;
using Camera::RIGHT;

/* 场景类 */
/* 世界类 一切参数在此设定 */
class Scene : public std::enable_shared_from_this<Scene> {
  public:
    /* 视口大小 */
    int imageWidth = 1600;
    int imageHeight = 900;
    int display_w = 0, display_h = 0;

    /* 时间 */
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    float curTime = 0.0f;
    float fps = 0.0f;
    bool showFPS = true;

    /* 光标位置 */
    float LastCursorX = 0;
    float LastCursorY = 0;
    /* 鼠标是否第一次进入窗口 */
    bool firstMouse = true;
    /* 鼠标是否在窗口内 */
    bool cursorInWindow = false;

    /* 窗口 */
    GLFWwindow *window = nullptr;

    /* 玩家 */
    shared_ptr<Player> player = make_shared<Player>(vec3(0.0f, 0.0f, 5.0f));

    /* 地图 */
    int mapX = 20 * 10;
    int mapY = 20;
    int mapZ = 20 * 10;
    vec3 ChunkSize = vec3(20, 20, 20);
    int CubeSize = 2.0f;
    shared_ptr<vector<vector<vector<shared_ptr<Cube>>>>> Map = make_shared<vector<vector<vector<shared_ptr<Cube>>>>>();
    vector<vector<vector<shared_ptr<Chunk>>>> Chunks;

    /* UI */
    App app = App(this);

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
    int InitGUI();

    void UpdateTimeAndFPS();
    void ProcessWindow();
    void MainRender();

    void ProcessKeyInput();
};

#endif