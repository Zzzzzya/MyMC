#include "Scene.hpp"
#include "Log/Logger.hpp"

/* 主世界 */
auto scene = make_shared<Scene>();

/* 鼠标绑定函数 */
void ProcessCursorPos(GLFWwindow *window, double xpos, double ypos);
void ProcessScroll(GLFWwindow *window, double xoffset, double yoffset);

int main(int argc, char **argv) {
    Loggers::Init();

    /* 世界初始化准备 */
    scene->InitScene(ProcessCursorPos, ProcessScroll);

    /* 世界大循环准备 - 包括游戏循环 & 渲染循环 */
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
    if (scene->app.state == App::State::RUN) {
        static float off = 0.0f;
        static float noff = 0.0f;
        if (yoffset > 0) {
            off += yoffset;
            noff = 0.0f;
        }
        else {
            noff += yoffset;
            off = 0.0f;
        }
        if (off > 1.0f) {
            scene->player->CurBlockID++;
            scene->player->CurBlockID %= CB_NUM;
            off = 0.0f;
        }
        if (noff < -1.0f) {
            scene->player->CurBlockID--;
            scene->player->CurBlockID = (scene->player->CurBlockID + CB_NUM) % CB_NUM;
            noff = 0.0f;
        }
    }
};