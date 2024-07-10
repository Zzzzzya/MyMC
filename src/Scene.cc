#include "Scene.hpp"

Scene::Scene() {
    int a;
}

void Scene::InitScene(void (*cursorPosCallback)(GLFWwindow *, double, double),
                      void (*scrollCallback)(GLFWwindow *, double, double)) {
    std::clog << " Init Scene" << std::endl;
    auto ResInitWindow = InitWindow(cursorPosCallback, scrollCallback);
    if (ResInitWindow == -1) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }

    std::clog << " Init Render" << std::endl;
    auto ResInitRender = InitRender();
    if (ResInitRender == -1) {
        cout << "Failed to init render" << endl;
        glfwTerminate();
    }

    std::clog << " Init Shaders" << std::endl;
    auto ResInitShaders = InitShaders();
    if (ResInitRender == -1) {
        cout << "Failed to init Shaders" << endl;
        glfwTerminate();
    }

    std::clog << " Init Textures" << std::endl;
    auto ResInitTextures = InitTextures();
    if (ResInitRender == -1) {
        cout << "Failed to init Textures" << endl;
        glfwTerminate();
    }

    std::clog << " Init Map" << std::endl;
    auto ResInitMeshes = InitMap();
    if (ResInitRender == -1) {
        cout << "Failed to init Meshes" << endl;
        glfwTerminate();
    }

    std::clog << " Init GUI" << std::endl;
    auto ResInitGUI = InitGUI();
    if (ResInitRender == -1) {
        cout << "Failed to init GUI" << endl;
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    return 0;
}

int Scene::InitMap() {
    // 初始化地图
    auto &map = *(this->Map);
    map.resize(mapX);
    for (auto &rol : map) {
        rol.resize(mapY);
        for (auto &cubes : rol) {
            cubes.resize(mapZ);
            for (auto &cube : cubes) {
                cube = make_shared<Cube>();
                cube->CubeID = 0;
            }
        }
    }

    for (auto &rol : map) {
        for (auto &cubes : rol) {
            for (auto &cube : cubes) {
                cube->CubeID = 1;
            }
        }
    }

    int dx[] = {1, -1, 0, 0, 0, 0};
    int dy[] = {0, 0, 1, -1, 0, 0};
    int dz[] = {0, 0, 0, 0, -1, 1};

    for (int i = 0; i < mapX; i++)
        for (int j = 0; j < mapY; j++)
            for (int k = 0; k < mapZ; k++) {
                if (map[i][j][k]->CubeID == 0)
                    continue;
                for (int p = 0; p < 6; p++) {
                    int nx = i + dx[p];
                    int ny = j + dy[p];
                    int nz = k + dz[p];
                    if (nx < 0 || nx >= mapX || ny < 0 || ny >= mapY || nz < 0 || nz >= mapZ ||
                        map[nx][ny][nz]->CubeID == 0) {
                        map[i][j][k]->Exposed[p] = true;
                    }
                    else {
                        map[i][j][k]->Exposed[p] = false;
                    }
                }
            }

    // 初始化Chunk
    Chunks.resize(1);
    Chunks[0] = make_shared<Chunk>(Map, vec3(0.0f, 0.0f, 0.0f), ChunkSize);

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
    ImGui_ImplOpenGL3_Init("#version 330");

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
        glm::perspective(glm::radians(player->camera.fov), (float)display_w / (float)display_h, 0.1f, 100.0f);

    auto cubeShader = Shader::GetDefaultShader(0);
    cubeShader->use();
    cubeShader->setMat4("view", view);
    cubeShader->setMat4("projection", projection);

    for (int i = 0; i < Texture::DefaultTexture.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, Texture::DefaultTexture[i]->id);
        cubeShader->setInt("tex[" + std::to_string(i) + "]", i);
    }

    for (int i = 0; i < Chunks.size(); i++) {
        Chunks[i]->Draw(view, projection, CubeSize);
    }
}

void Scene::ProcessKeyInput() {
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (cursorInWindow) {
                cout << "ESC Pressed" << endl;
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
