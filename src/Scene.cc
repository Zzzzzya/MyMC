#include "Scene.hpp"

Scene::Scene() {
}

void Scene::InitScene(void (*cursorPosCallback)(GLFWwindow *, double, double),
                      void (*scrollCallback)(GLFWwindow *, double, double)) {
    auto ResInitWindow = InitWindow(cursorPosCallback, scrollCallback);
    if (ResInitWindow == -1) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }

    auto ResInitRender = InitRender();
    if (ResInitRender == -1) {
        cout << "Failed to init render" << endl;
        glfwTerminate();
    }

    auto ResInitShaders = InitShaders();
    if (ResInitRender == -1) {
        cout << "Failed to init Shaders" << endl;
        glfwTerminate();
    }

    auto ResInitTextures = InitTextures();
    if (ResInitRender == -1) {
        cout << "Failed to init Textures" << endl;
        glfwTerminate();
    }

    auto ResInitMeshes = InitMap();
    if (ResInitRender == -1) {
        cout << "Failed to init Meshes" << endl;
        glfwTerminate();
    }

    auto ResInitGUI = InitGUI();
    if (ResInitRender == -1) {
        cout << "Failed to init GUI" << endl;
        glfwTerminate();
    }
}

void Scene::MainLoop() {
    /* Render Loop */
    while (!glfwWindowShouldClose(window)) {
        /* Time Update */
        curTime = glfwGetTime();
        deltaTime = curTime - lastTime;
        lastTime = curTime;

        // 开始新的ImGui帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 获取主视口
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags host_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                             ImGuiWindowFlags_NoNavFocus;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", NULL, host_window_flags);
        ImGui::PopStyleVar(3);

        // 创建DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();

        // 创建一个窗口
        ImGui::Begin("Example Window");
        ImGui::Text("This is an example window.");
        // 创建一个无边框按钮
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        if (ImGui::Button("No Border Button")) {
            // 按钮点击事件
        }
        ImGui::PopStyleVar();
        ImGui::End();

        // 渲染ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        /* Main Render */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // MVPs
        mat4 view = player->camera.ViewMat();
        mat4 projection =
            glm::perspective(glm::radians(player->camera.fov), (float)display_w / (float)display_h, 0.1f, 100.0f);

        // Draw
        for (int i = 0; i < mapX; i++) {
            for (int j = 0; j < mapY; j++) {
                for (int k = 0; k < mapZ; k++) {
                    Shaders[0]->use();
                    Shaders[0]->setMVPS(Map[i][j][k]->getModel(), view, projection);
                    Map[i][j][k]->Draw(Shaders[0]);
                }
            }
        }

        /* ImGui Render */
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        // Poll and handle events
        glfwPollEvents();

        ProcessKeyInput();
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    Shaders.push_back(make_shared<Shader>("MVP.vs", "Cube.fs")); // 0 for Cube Texture Shader
    return 0;
}

int Scene::InitTextures() {
    GrassBlock::loadTextures();
    return 0;
}

int Scene::InitMap() {
    Map.resize(mapX);
    for (auto &rol : Map) {
        rol.resize(mapY);
        for (auto &meshes : rol) {
            meshes.resize(mapZ);
            for (auto &mesh : meshes) {
                mesh = make_shared<GrassBlock>();
            }
        }
    }
    for (int i = 0; i < mapX; i++)
        for (int j = 0; j < mapY; j++)
            for (int k = 0; k < mapZ; k++) {
                Map[i][j][k]->translate = vec3(i * CubeSize, j * CubeSize, -k * CubeSize);
                // Map[i][j][k]->scale = vec3(0.5f);
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
    ImGui_ImplOpenGL3_Init("#version 330");

    return 0;
}
