#include "Scene.hpp"

Scene::Scene() {
    map = make_shared<Map>(vec3(mapX, mapY, mapZ));
}

void Scene::InitScene(void (*cursorPosCallback)(GLFWwindow *, double, double),
                      void (*scrollCallback)(GLFWwindow *, double, double)) {
    // 获取日志系统
    auto logger = Loggers::getLogger("Scene");

    LOG_INFO(logger, "Init Scene");
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

    LOG_INFO(logger, "Init Shaders");
    auto ResInitShaders = InitShaders();
    if (ResInitRender == -1) {
        LOG_ERROR(logger, "Failed to init Shaders");
        glfwTerminate();
    }

    LOG_INFO(logger, "Init Textures");
    auto ResInitTextures = InitTextures();
    if (ResInitRender == -1) {
        LOG_ERROR(logger, "Failed to init Textures");
        glfwTerminate();
    }

    LOG_INFO(logger, "Init Map");
    auto ResInitMeshes = InitMap();
    if (ResInitRender == -1) {
        LOG_ERROR(logger, "Failed to init Map");
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
    /* Render Loop */
    while (!glfwWindowShouldClose(window)) {
        /* 更新时间 */
        UpdateTimeAndFPS();

        app.PrepareRender();

        /* 渲染 */
        if (app.state == App::State::RUN)
            MainRender();

        app.ImGuiRender();

        /* window 处理 */
        ProcessWindow();
    }
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
    glfwSwapInterval(1); // Enable vsync

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    return 0;
}

int Scene::InitRender() {
    // GLEW Init
    glewInit();

    // 渲染初始化参数设置
    glEnable(GL_DEPTH_TEST);

    return 0;
}

int Scene::InitShaders() {
    Shader::setUpDefaultShaders();
    return 0;
}

int Scene::InitTextures() {
    Texture::setUpDefaultTextures();
    CubeMap::setUpDefaultCubeMaps();
    return 0;
}

int Scene::InitMap() {
    map->InitMap();

    // 初始化Chunk
    auto logger = Loggers::getLogger("Scene");
    int ChunkX = mapX / ChunkSize.x;
    int ChunkY = mapY / ChunkSize.y;
    int ChunkZ = mapZ / ChunkSize.z;
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

void Scene::MainRender() {
    /* Main Render 真正渲染场景的地方 */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // MVPs
    mat4 view = player->camera.ViewMat();
    mat4 projection =
        glm::perspective(glm::radians(player->camera.fov), (float)display_w / (float)display_h, 0.1f, 1000.0f);

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
        // glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap::DefaultCubeMaps[i - texNum]->id);
        // cubeShader->setInt("tex[" + std::to_string(i - texNum) + "]", i);
        cubeShader->setHandle("tex[" + std::to_string(i) + "]", CubeMap::DefaultCubeMaps[i]->handle);
    }

    for (auto &rol : Chunks)
        for (auto &cubes : rol)
            for (auto &chunk : cubes) {
                chunk->Draw(view, projection, 2.0f);
            }
}

void Scene::ProcessKeyInput() {
    {
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
}
