#include "Scene.hpp"
#include "Log/Logger.hpp"

auto scene = make_shared<Scene>();

/* 鼠标绑定函数 */
void ProcessCursorPos(GLFWwindow *window, double xpos, double ypos);
void ProcessScroll(GLFWwindow *window, double xoffset, double yoffset);

int main(int argc, char **argv) {
    Loggers::init();

    scene->InitScene(ProcessCursorPos, ProcessScroll);

    scene->MainLoop();

    scene->DestroyScene();
    return 0;
}

/* GLFW 鼠标绑定函数 */
void ProcessCursorPos(GLFWwindow *window, double xpos, double ypos) {
    if (!scene->cursorInWindow) {
        return;
    }

    if (scene->firstMouse) {
        scene->LastCursorX = static_cast<float>(xpos);
        scene->LastCursorY = static_cast<float>(ypos);
        scene->firstMouse = false;
    }
    float offsetx = static_cast<float>(xpos - scene->LastCursorX);
    float offsety = static_cast<float>(scene->LastCursorY - ypos);
    scene->LastCursorX = static_cast<float>(xpos);
    scene->LastCursorY = static_cast<float>(ypos);

    scene->player->camera.ProcessCursorPos(offsetx, offsety);
};
void ProcessScroll(GLFWwindow *window, double xoffset, double yoffset) {
    if (!scene->cursorInWindow)
        return;
    scene->player->camera.ProcessScroll(yoffset);
};