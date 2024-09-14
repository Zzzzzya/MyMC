#include "Application.hpp"
#include "Scene.hpp"

App::App(Scene *scene) : scene(scene) {
}

void App::PrepareRender() {

    // 开始新的ImGui帧
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    switch (state) {
    case State::INIT:
        Init();
        break;
    case State::SETTING:
        Setting();
        break;
    case State::RUN:
        Run();
        break;
    case State::WAITING:
        Init();
        break;
    default:
        break;
    }

    // 渲染ImGui
    ImGui::Render();

    glfwGetFramebufferSize(scene->window, &scene->display_w, &scene->display_h);
    glViewport(0, 0, scene->display_w, scene->display_h);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::ImGuiRender() {
    /* ImGui Render */
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::Init() {
    // 创建一个窗口
    ImGui::Begin("Example Window");
    ImGui::Text("FPS: %.1f", scene->fps);
    // 创建一个无边框按钮
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    if (ImGui::Button("In Game!")) {
        state = State::RUN;
        glfwSetInputMode(scene->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene->cursorInWindow = true;
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void App::Setting() {
}

void App::Run() {
    if (scene->showFPS) {
        ImGui::Begin("FPS");
        ImGui::Text("FPS: %.1f", scene->fps);
        ImGui::Text("Selected position: (%.1f, %.1f, %.1f)", scene->SelectedBlockToDo.x, scene->SelectedBlockToDo.y,
                    scene->SelectedBlockToDo.z);
        static auto N = vec3(0.0f, 1.0f, 0.0f);
        auto trueFront = normalize(scene->player->camera.front - dot(scene->player->camera.front, N) * N);
        ImGui::Text("Camera Front: (%.1f, %.1f, %.1f)", trueFront.x, trueFront.y, trueFront.z);
        ImGui::End();
    }
}

void App::Waiting() {
}
