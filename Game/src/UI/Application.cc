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
        Waiting();
        break;
    case State::CREATING_NEW_GAME:
        CreatingNewGame();
        break;
    case State::PICTURE:
        Picture();
        break;
    default:
        break;
    }

    // 渲染ImGui
    ImGui::Render();

    glfwGetFramebufferSize(scene->window, &scene->display_w, &scene->display_h);
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

    /**
     * @brief 主界面渲染逻辑
     * @button New Game 新游戏
     * @button Continue Game 继续游戏
     * @button Load Game 导入游戏
     * @button Setting 设置
     *
     */
    if (ImGui::Button("New Game!")) {
        state = State::CREATING_NEW_GAME;
        glfwSetInputMode(scene->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene->cursorInWindow = true;
    }
    if (ImGui::Button("Continue Game!")) {
        // TODO：继续上一次游戏
        /**
         * @brief Step 1 ： Check if there is a saved game
         * @brief Step 2 ： Load the saved game
         */
    }
    if (ImGui::Button("Load Game!")) {
        // TODO：加载某个游戏存档
        /**
         * @brief
         */
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void App::Setting() {
    // 创建一个窗口
    ImGui::Begin("Setting Window");
    ImGui::Text("FPS: %.1f", scene->fps);
    // 创建一个无边框按钮
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    /**
     * @brief 主界面渲染逻辑
     * @button Continue Game 继续游戏
     *
     */
    if (ImGui::Button("Continue Game!")) {
        state = State::RUN;
        glfwSetInputMode(scene->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene->cursorInWindow = true;
    }
    ImGui::PopStyleVar();

    /**
     * @brief 调节Scene中的参数
     *
     */

    // 1. 是否开启垂直同步
    ImGui::Checkbox("VSync", &scene->bVSync);
    glfwSwapInterval(scene->bVSync);

    // 2. 雾效设置
    ImGui::SliderFloat("fogDensity", &scene->fogDensity, 0.0f, 0.1f);

    ImGui::SliderFloat("Sun.x", &scene->SunLightVec.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Sun.y", &scene->SunLightVec.y, -1.0f, 1.0f);
    ImGui::SliderFloat("Sun.z", &scene->SunLightVec.z, -1.0f, 1.0f);
    ImGui::SliderFloat("Sun.front.x", &scene->SunPosition.x, -10.0f, 100.0f);
    ImGui::SliderFloat("Sun.front.y", &scene->SunPosition.y, 80.0f, 300.0f);
    ImGui::SliderFloat("Sun.front.z", &scene->SunPosition.z, -100.0f, 10.0f);
    ImGui::SliderFloat("shadowbias", &scene->shadowBias, .0f, 2.f);

    if (ImGui::Button("Back")) {
        state = State::WAITING;
    }
    ImGui::End();
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
    // 创建一个窗口
    ImGui::Begin("Example Window");
    ImGui::Text("FPS: %.1f", scene->fps);
    // 创建一个无边框按钮
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    /**
     * @brief 主界面渲染逻辑
     * @button Continue Game 继续游戏
     *
     */
    if (ImGui::Button("Continue Game!")) {
        state = State::RUN;
        glfwSetInputMode(scene->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene->cursorInWindow = true;
    }
    ImGui::PopStyleVar();

    if (ImGui::Button("Settings")) {
        state = State::SETTING;
    }
    ImGui::End();
}

void App::CreatingNewGame() {
    // 创建一个窗口
    ImGui::Begin("Example Window");

    ImGui::Text("CREATING NEW WORLD! %d", 1);

    if (scene->bMapReady) {
        state = State::RUN;
    }

    // ImGui::PopStyleVar();
    ImGui::End();
}

void App::Picture() {
    ImGui::Begin("Picture Mode");
    ImGui::End();
}
