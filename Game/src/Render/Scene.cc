#include "Scene.hpp"

Scene::Scene() {
    map = make_shared<Map>(vec3(mapX, mapY, mapZ));
}

void Scene::InitScene(void (*cursorPosCallback)(GLFWwindow *, double, double),
                      void (*scrollCallback)(GLFWwindow *, double, double)) {
    // 获取日志系统
    auto logger = Loggers::getLogger("Scene");

    LOG_INFO(logger, "Init Scene");

    LOG_INFO(logger, "Init Window");
    auto ResInitWindow = InitWindow(cursorPosCallback, scrollCallback);
    if (ResInitWindow == -1) {
        LOG_ERROR(logger, "Failed to create GLFW window");
        glfwTerminate();
    }

    LOG_INFO(logger, "Init Render");
    auto ResInitRender = InitRender();
    if (ResInitRender == -1) {
        LOG_ERROR(logger, "Failed to init Render");
        glfwTerminate();
    }

    LOG_INFO(logger, "Init GUI");
    auto ResInitGUI = InitGUI();
    if (ResInitRender == -1) {
        LOG_ERROR(logger, "Failed to init GUI");
        glfwTerminate();
    }
}

/* 主渲染循环 */
void Scene::MainLoop() {
    // 获取日志系统
    auto logger = Loggers::getLogger("MainLoop");

    /* Render Loop */
    while (!glfwWindowShouldClose(window)) {
        /* 更新时间 */
        UpdateTimeAndFPS();

        app.PrepareRender();

        /* 渲染 */
        if (app.state == App::State::CREATING_NEW_GAME) {
            CreatingNewGame();
        }

        if (app.state == App::State::RUN || app.state == App::State::PICTURE)
            MainRender();

        app.ImGuiRender();

        /* window 处理 */
        ProcessWindow();

        /* GamePlay */
        // 1. 射线追踪 找到当前对准的方块
        if (app.state == App::State::RUN)
            SelectedAnyBlock = map->ViewRayTrace(player->camera.position, player->camera.front, SelectedBlockToDo,
                                                 SelectedBlockToAdd, viewRayTraceDistance, viewRayTraceStep);
    }
}

void Scene::CreatingNewGame() {
    // 获取日志系统
    auto logger = Loggers::getLogger("CreatingNewGame");
    static int state = 0;

    LOG_INFO(logger, "Init Shaders");
    auto ResInitShaders = InitShaders();
    if (ResInitShaders == -1) {
        LOG_ERROR(logger, "Failed to init Shaders");
        glfwTerminate();
    }

    LOG_INFO(logger, "Init Textures");
    auto ResInitTextures = InitTextures();
    if (ResInitTextures == -1) {
        LOG_ERROR(logger, "Failed to init Textures");
        glfwTerminate();
    }

    LOG_INFO(logger, "Init Map");
    auto ResInitMeshes = InitMap();
    if (ResInitMeshes == -1) {
        LOG_ERROR(logger, "Failed to init Map");
        glfwTerminate();
    }

    app.state = App::State::RUN;
}

void Scene::MainRender() {
    /* Main Render 真正渲染场景的地方 */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SceneCulling();

    UpdateVP();

    CubeShaderDraw();

    SelectedBlockShaderDraw();

    SparkShaderDraw();
}

void Scene::DestroyScene() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int Scene::InitWindow(void (*cursorPosCallback)(GLFWwindow *, double, double),
                      void (*scrollCallback)(GLFWwindow *, double, double)) {
    // 初始化GLFW 和 窗口
    // Setup GLFW
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window with GLFW
    window = glfwCreateWindow(imageWidth, imageHeight, "MC", NULL, NULL);
    if (window == NULL)
        return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(bVSync); // Enable vsync

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    return 0;
}

static vector<Vertex2D> sparkVertices = {{vec2(-0.5f, -0.5f), vec2(0.0f, 0.0f)}, {vec2(0.5f, -0.5f), vec2(1.0f, 0.0f)},
                                         {vec2(0.5f, 0.5f), vec2(1.0f, 1.0f)}, // 1
                                         {vec2(0.5f, 0.5f), vec2(1.0f, 1.0f)},   {vec2(-0.5f, 0.5f), vec2(0.0f, 1.0f)},
                                         {vec2(-0.5f, -0.5f), vec2(0.0f, 0.0f)}};
int Scene::InitRender() {
    // GLEW Init
    glewInit();

    // 渲染初始化参数设置
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_CULL_FACE);  // 开启背面剔除

    // 选中物体渲染设置
    glGenVertexArrays(1, &SelectedBlockVAO);
    glGenBuffers(1, &SelectedBlockVBO);

    // 光标渲染设置
    glGenVertexArrays(1, &sparkVAO);
    glGenBuffers(1, &sparkVBO);
    glBindVertexArray(sparkVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sparkVBO);
    glBufferData(GL_ARRAY_BUFFER, sparkVertices.size() * sizeof(Vertex2D), &sparkVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 0;
}

int Scene::InitShaders() {
    Shader::setUpDefaultShaders();
    return 0;
}

int Scene::InitTextures() {
    Texture::setUpDefaultTextures();
    CubeMap::setUpDefaultCubeMaps();
    spark = make_shared<Texture>("spark.png", "../res/textures/");
    return 0;
}

int Scene::InitMap() {
    map->InitMap();

    // 初始化Chunk
    auto logger = Loggers::getLogger("Scene");
    int ChunkX = MAX_CHUNK_X;
    int ChunkY = MAX_CHUNK_Y;
    int ChunkZ = MAX_CHUNK_Z;
    Chunks.resize(ChunkX, vector<vector<shared_ptr<Chunk>>>(ChunkY, vector<shared_ptr<Chunk>>(ChunkZ)));
    for (int i = 0; i < ChunkX; i++)
        for (int j = 0; j < ChunkY; j++)
            for (int k = 0; k < ChunkZ; k++) {
                vec3 position = vec3(i, j, k);
                Chunks[i][j][k] = make_shared<Chunk>(map, position, ChunkSize);
            }

    return 0;
}

int Scene::InitGUI() {
    // 初始化GUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // 启动Docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // 设置GUI风格
    ImGui::StyleColorsDark();

    // 设置GUI渲染器
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    return 0;
}

void Scene::UpdateTimeAndFPS() {
    /* 时间和帧率的更新 */
    curTime = glfwGetTime();
    deltaTime = curTime - lastTime;
    lastTime = curTime;
    static int frameCount = 0;
    static float timeCount = 0;
    timeCount += deltaTime;
    frameCount++;
    if (timeCount >= 1.0f) {
        fps = frameCount / timeCount;
        frameCount = 0;
        timeCount = 0;
    }
}

void Scene::ProcessWindow() {
    glfwSwapBuffers(window);
    glfwPollEvents();
    ProcessKeyInput();
}

static inline bool KeyPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void Scene::SceneCulling() {
    if (app.state == App::State::PICTURE)
        return;

    // 更新视锥体
    frustum = Frustum(player->camera, (float)display_w / (float)display_h);

    // 场景剔除
    for (auto &rol : Chunks)
        for (auto &cubes : rol)
            for (auto &chunk : cubes) {
                // 距离剔除 + 视锥剔除
                chunk->isCulled = FrustumCulling(*chunk) || DistanceCulling(*chunk);
            }
}

bool Scene::DistanceCulling(Chunk &chunk) {

    vec3 WorldPos = chunk.pos * ChunkSize * 2.0f;
    static auto i = ChunkSize.x / 2;
    static auto j = ChunkSize.y / 2;
    static auto k = ChunkSize.z / 2;
    WorldPos.z = -WorldPos.z;
    auto chunkCenter = WorldPos + vec3(i, j, -k) * 2.0f;

    // Compute Z distance
    vec4 ViewSpace = view * vec4(chunkCenter, 1.0f);

    if (abs(ViewSpace.z) > maxCullingDistance)
        return true;
    else
        return false;
}

bool Scene::FrustumCulling(Chunk &chunk) {
    // AABB
    vec3 WorldPos = chunk.pos * ChunkSize * 2.0f;
    WorldPos.z = -WorldPos.z;
    vec3 NextPos = WorldPos + vec3(ChunkSize.x, ChunkSize.y, -ChunkSize.z) * 2.0f;
    AABB box(WorldPos, NextPos);

    return !frustum.isAABBInFrustum(box);
}

void Scene::UpdateVP() {
    // MVPs
    view = player->camera.ViewMat();
    projection = glm::perspective(glm::radians(player->camera.fov), (float)display_w / (float)display_h, 0.1f, 1000.0f);
}

void Scene::CubeShaderDraw() {
    auto cubeShader = Shader::GetDefaultShader(0);
    cubeShader->use();
    cubeShader->setMat4("view", view);
    cubeShader->setMat4("projection", projection);

    int texNum = 0;
    for (int i = 0; i < Texture::DefaultTexture.size(); i++) {
        // glActiveTexture(GL_TEXTURE0 + i);
        // glBindTexture(GL_TEXTURE_2D, Texture::DefaultTexture[i]->id);
        // cubeShader->setInt("tex2D[" + std::to_string(i) + "]", i);
        cubeShader->setHandle("tex2D[" + std::to_string(i) + "]", Texture::DefaultTexture[i]->handle);
        texNum++;
    }

    for (int i = 0; i < CubeMap::DefaultCubeMaps.size(); i++) {
        // glActiveTexture(GL_TEXTURE0 + i);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap::DefaultCubeMaps[i -
        // texNum]->id); cubeShader->setInt("tex[" + std::to_string(i - texNum) +
        // "]", i);
        cubeShader->setHandle("tex[" + std::to_string(i) + "]", CubeMap::DefaultCubeMaps[i]->handle);
    }

    cubeShader->setVec3("cameraPos", player->camera.position);
    cubeShader->setFog(vec3(1.0f, 1.0f, 1.0f), 300.0f, 1000.0f, fogDensity);

    for (auto &rol : Chunks)
        for (auto &cubes : rol)
            for (auto &chunk : cubes) {
                chunk->Draw(view, projection, 2.0f);
            }
}

void Scene::SelectedBlockShaderDraw() {
    if (app.state == App::State::PICTURE)
        return;

    // 渲染选中物体
    // TODO: 预选物体渲染抽离到函数
    if (SelectedAnyBlock) {
        if (SelectedBlockVertices.size() != 0) {
            SelectedBlockVertices.clear();
        }
        if (SelectedBlockToDo.x < 0 || SelectedBlockToDo.y < 0 || SelectedBlockToDo.z < 0)
            return;
        auto cube = (*(map->_map))[SelectedBlockToDo.x][SelectedBlockToDo.y][SelectedBlockToDo.z];
        auto WorldPos = vec3(SelectedBlockToDo.x, SelectedBlockToDo.y, -SelectedBlockToDo.z) * 2.0f;
        cube->GenerateVertices(SelectedBlockVertices, WorldPos);
        if (SelectedBlockVertices.size() != 0) {
            glBindVertexArray(SelectedBlockVAO);
            glBindBuffer(GL_ARRAY_BUFFER, SelectedBlockVBO);
            glBufferData(GL_ARRAY_BUFFER, SelectedBlockVertices.size() * sizeof(Vertex), &SelectedBlockVertices[0],
                         GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

            auto shader = Shader::GetDefaultShader(2);
            shader->use();
            shader->setMat4("view", view);
            shader->setMat4("projection", projection);
            shader->setVec3("color", vec3(1.0f, 0.0f, 0.0f));
            glBindVertexArray(SelectedBlockVAO);
            glDrawArrays(GL_TRIANGLES, 0, SelectedBlockVertices.size());

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }
}

void Scene::SparkShaderDraw() {
    // 渲染光标
    // TODO：光标渲染抽离到函数
    glDisable(GL_DEPTH_TEST);
    auto sparkShader = Shader::GetDefaultShader(1);
    static mat4 sparkModel = glm::scale(glm::mat4(1.0f), vec3((float)imageHeight / imageWidth, 1.0f, 1.0f) * 0.035f);
    sparkShader->use();
    sparkShader->setMat4("model", sparkModel);
    sparkShader->setHandle("spark", spark->handle);
    glBindVertexArray(sparkVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void Scene::ProcessKeyInput() {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (cursorInWindow) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorInWindow = false;
            firstMouse = true;
            app.state = App::State::WAITING;
        }
    }
    if (!cursorInWindow)
        return;

    ProcessMovement();

    static bool isJump = false;
    static float jumpTime = 0.0f;
    if (isJump == false && KeyPressed(window, GLFW_KEY_SPACE)) {
        if (isJump == false && jumpTime <= 1.0f) {
            isJump = true;
            jumpTime += deltaTime;
            player->camera.ProcessJump(deltaTime, map);
        }
    }
    else {
        isJump = false;
        jumpTime = 0.0f;
        player->camera.ProcessDropDown(deltaTime, map);
    }

    if (KeyPressed(window, GLFW_KEY_F2)) {
        app.state = App::State::RUN;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursorInWindow = true;
    }

    if (KeyPressed(window, GLFW_KEY_F1)) {
        app.state = App::State::SETTING;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursorInWindow = false;
    }

    if (KeyPressed(window, GLFW_KEY_P)) {
        if (app.state == App::State::RUN)
            app.state = App::State::PICTURE;
    }

    if (KeyPressed(window, GLFW_KEY_O)) {
        if (app.state == App::State::PICTURE)
            app.state = App::State::RUN;
    }
}

static vector<int> MoveKey = {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D};

void Scene::ProcessMovement() {
    for (int i = 0; i < MoveKey.size(); i++) {
        if (glfwGetKey(window, MoveKey[i]) == GLFW_PRESS) {
            player->camera.ProcessKeyBoard(static_cast<Camera::Movement>(i), deltaTime, map);
        }
    }
}
