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
}

void Scene::MainLoop() {
    /* Render Loop */
    while (!glfwWindowShouldClose(window)) {
        /* Time Update */
        curTime = glfwGetTime();
        deltaTime = curTime - lastTime;
        lastTime = curTime;

        /* ViewPort Set */
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        if (display_h < 1)
            display_h = 1;
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
