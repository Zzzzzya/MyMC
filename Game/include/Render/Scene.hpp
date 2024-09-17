#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

#include "Header.hpp"
#include "Shader.hpp"
#include "mTexture.hpp"
#include "Mesh.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Frustum.hpp"
#include "FrameBuffers.hpp"

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
    shared_ptr<Player> player = make_shared<Player>(vec3(20.0f, 100.0f, -20.0f));

    // vec3 SunLightVec = vec3(1.0f, 0.0f, -1.0f);

    /* 地图 */
    int mapX = 16 * 25;
    int mapY = 100 * 1;
    int mapZ = 16 * 25;
    vec3 ChunkSize = vec3(16, 100, 16);
    int CubeSize = 2.0f;

    int MAX_CHUNK_X = 25;
    int MAX_CHUNK_Y = 1;
    int MAX_CHUNK_Z = 25;

    shared_ptr<Map> map;
    vector<vector<vector<shared_ptr<Chunk>>>> Chunks;

    /* UI */
    App app = App(this);
    bool bBeginCreatingNewGame = false;
    bool bMapReady{false};
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

    /* Render相关 */
    mat4 view;
    mat4 projection;
    float maxCullingDistance = 500.0f;
    Frustum frustum;
    ScreenQuad screenQuad;
    shared_ptr<FrameBufferDepthMap> shadowMap;
    SunChunk sunChunk;
    vec3 SunPosition = vec3(0.0f, 100.0f, 0.0f);
    vec3 SunLightVec = vec3(1.0f, -1.0f, -1.0f);
    float shadowBias = 0.088f;
    mat4 lightMatrix;

    /* 可控类Setting */
    bool bVSync = false;
    float fogDensity = 0.003f;

    Scene();

    /**
     * @brief 初始化场景 - 依次调度所有初始化函数
     *
     * @param cursorPosCallback
     * @param scrollCallback
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

    void CreatingNewGame();
    void MainRender();

    /**
     * @brief MainRender子函数集合
     *
     */
    void SceneCulling(); // 场景剔除
    void UpdateVP();
    void ShadowMapDraw();
    void CubeShaderDraw();
    void WaterDraw();
    void CloudDraw();
    void SkyDraw();
    void SunDraw();
    void SelectedBlockShaderDraw();
    void SparkShaderDraw();

    /**
     * @brief 场景剔除子模块
     *
     */
    bool DistanceCulling(Chunk &); // 距离剔除
    bool FrustumCulling(Chunk &);  // 视锥剔除

    void ProcessKeyInput();
    void ProcessMovement();
};

#endif