#include "Scene.hpp"

Scene scene;

int main(int argc, char **argv) {
    // Setup GLFW
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with GLFW
    scene.window = glfwCreateWindow(scene.imageWidth, scene.imageHeight, "MC", NULL, NULL);
    if (scene.window == NULL)
        return -1;
    glfwMakeContextCurrent(scene.window);
    glfwSwapInterval(1); // Enable vsync

    // GLEW Init
    glewInit();

    // Application state  -->   prepare our models
    glEnable(GL_DEPTH_TEST);

    // Vertex
    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Shader
    Shader SingleColorShader("Nothing.vs", "SingleColor.fs");
    auto CubeShader = make_shared<Shader>("MVP.vs", "Cube.fs");

    // Model
    GrassBlock::loadTextures();
    GrassBlock grassBlock;

    /* Render Loop */
    while (!glfwWindowShouldClose(scene.window)) {
        /* Time Update */
        scene.curTime = glfwGetTime();
        scene.deltaTime = scene.curTime - scene.lastTime;
        scene.lastTime = scene.curTime;

        /* ViewPort Set */
        int display_w, display_h;
        glfwGetFramebufferSize(scene.window, &display_w, &display_h);
        if (display_h < 1)
            display_h = 1;
        glViewport(0, 0, display_w, display_h);

        /* Main Render */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // MVPs
        mat4 view = scene.player->camera.ViewMat();
        mat4 projection =
            glm::perspective(glm::radians(scene.player->camera.fov), (float)display_w / (float)display_h, 0.1f, 100.0f);

        // Draw
        CubeShader->setMVPS(glm::mat4(1.0f), view, projection);
        grassBlock.Draw(CubeShader);

        glfwSwapBuffers(scene.window);
        // Poll and handle events
        glfwPollEvents();
    }

    glfwDestroyWindow(scene.window);
    glfwTerminate();

    return 0;
}
