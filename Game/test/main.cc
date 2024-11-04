#include "Scene.hpp"
#include "Log/Logger.hpp"
#include "SDL_mixer.h"
#include <boost/asio.hpp>

/* 主世界 */
auto scene = make_shared<Scene>();

/* 鼠标绑定函数 */
void ProcessCursorPos(GLFWwindow *window, double xpos, double ypos);
void ProcessScroll(GLFWwindow *window, double xoffset, double yoffset);

void StartNetworking() {
    boost::asio::io_context io_context;

    // 这里可以添加服务器或客户端的初始化代码
    // 例如，创建一个TCP连接
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("localhost", "daytime");

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    // 这里可以添加数据发送和接收的代码
}

int main(int argc, char **argv) {
    Loggers::init();

    // 启动网络功能
    StartNetworking();

    // 初始化音频
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    // 加载背景音乐
    Mix_Music *bgm = Mix_LoadMUS("path/to/background_music.mp3");
    if (bgm == nullptr) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    // 播放背景音乐
    Mix_PlayMusic(bgm, -1);

    /* 世界初始化准备 */
    scene->InitScene(ProcessCursorPos, ProcessScroll);

    /* 世界大循环准备 - 包括游戏循环 & 渲染循环 */
    scene->MainLoop();

    // 释放音乐资源
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();

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
}

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
}