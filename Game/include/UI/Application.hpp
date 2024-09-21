#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Header.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Scene;
class App
{
public:
  enum class State
  {
    INIT,
    CREATING_NEW_GAME,
    RUN,
    SETTING,
    WAITING,
    PICTURE // 拍照模式
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
  void CreatingNewGame();
  void Picture();
  int GetSceneWidth();
  int GetSceneHeight();
  // begin window and change style of windows and buttons
  ImVec2 BeginWindow(std::string title, int BtnCount, bool setting = false); 
  // end window
  void EndWindow();
};

#endif