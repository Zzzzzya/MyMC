#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Header.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Scene;
class App {
  public:
    enum class State {
        INIT,
        RUN,
        SETTING,
        WAITING
    };

    State state = State::INIT;
    Scene *scene = nullptr;

    App(Scene *scene);
    void PrepareRender();
    void ImGuiRender();

  private:
    void Init(); // Start
    void Setting();
    void Run();
    void Waiting();
};

#endif