#include "Header.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>

int imageWidth = 1600;
int imageHeight = 900;

float curTime = 0.0f;
float lastTime = 0.0f;
float deltaTime = 0.0f;

int main(int argc, char **argv) {
    // Setup GLFW
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with GLFW
    GLFWwindow *window = glfwCreateWindow(imageWidth, imageHeight, "MC", NULL, NULL);
    if (window == NULL)
        return -1;
    glfwMakeContextCurrent(window);
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

        SingleColorShader.use();
        SingleColorShader.setVec3(
            "color", vec3(cos(radians(curTime) * 100) * 0.5 + 0.5, sin(radians(curTime) * 100) * 0.5 + 0.5, 0.0f));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        // Poll and handle events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
