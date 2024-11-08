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
    try {
        boost::asio::io_context io_context;

        // 解析服务器地址和端口
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("localhost", "12345");

        // 创建并连接套接字
        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // 发送数据
        std::string request = "Hello from client!";
        boost::asio::write(socket, boost::asio::buffer(request));

        // 接收数据
        char reply[1024];
        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, request.size()));
        std::cout << "Reply from server: ";
        std::cout.write(reply, reply_length);
        std::cout << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
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