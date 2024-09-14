#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

#include "Header.hpp"
#include "Shader.hpp"
#include "mTexture.hpp"
#include "Mesh.hpp"
#include "Player.hpp"
#include "Map.hpp"

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
    shared_ptr<Player> player = make_shared<Player>(vec3(0.0f, 100.0f, 0.0f));

    /* 地图 */
    int mapX = 40 * 8;
    int mapY = 20 * 10;
    int mapZ = 40 * 8;
    vec3 ChunkSize = vec3(40, 20, 40);
    int CubeSize = 2.0f;

    shared_ptr<Map> map;
    vector<vector<vector<shared_ptr<Chunk>>>> Chunks;

    /* UI */
    App app = App(this);
    shared_ptr<Texture> spark;
    unsigned int sparkVAO, sparkVBO;

    /* GamePlay相关 */
    float viewRayTraceDistance = 10.0f;
    float viewRayTraceStep = 0.1f;
    bool SelectedAnyBlock = false;
    vec3 SelectedBlockToDo = vec3(0.0f, 0.0f, 0.0f);
    vec3 SelectedBlockToAdd = vec3(0.0f, 0.0f, 0.0f);
    unsigned int SelectedBlockVAO, SelectedBlockVBO;
    vector<Vertex> SelectedBlockVertices;

    Scene();

    /**
     * @brief 初始化场景 - 依次调度所有初始化函数
     *
     * @param cursorPosCallback
     * @param scrollCallback
     * TODO: 初始化分离，游戏选择页面 -> 新游戏再初始化
     */
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
    void ProcessMovement();
};

#endif