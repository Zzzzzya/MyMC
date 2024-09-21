#include "Application.hpp"
#include "Scene.hpp"

App::App(Scene *scene) : scene(scene)
{
}

void App::PrepareRender()
{

    // 开始新的ImGui帧
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    switch (state)
    {
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

void App::ImGuiRender()
{
    /* ImGui Render */
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::Init()
{
    // 创建一个window并设置window和buttons的样式
    ImVec2 BtnSize = BeginWindow("Example Window", 3);

    /**
     * @brief 主界面渲染逻辑
     * @button New Game 新游戏
     * @button Continue Game 继续游戏
     * @button Load Game 导入游戏
     * @button Setting 设置
     *
     */
    if (ImGui::Button("New Game!", BtnSize))
    {
        state = State::CREATING_NEW_GAME;
        glfwSetInputMode(scene->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene->cursorInWindow = true;
    }
    
    ImGui::Spacing();
    // ImGui::SetCursorPosX(startX);
    if (ImGui::Button("Continue Game!", BtnSize))
    {
        // TODO：继续上一次游戏
        /**
         * @brief Step 1 ： Check if there is a saved game
         * @brief Step 2 ： Load the saved game
         */
    }

    ImGui::Spacing();
    // ImGui::SetCursorPosX(startX);
    if (ImGui::Button("Load Game!", BtnSize))
    {
        // TODO：加载某个游戏存档
        /**
         * @brief
         */
    }
    // 关闭窗口
    EndWindow();
}

void App::Setting()
{
    // 创建一个窗口
    ImVec2 BtnSize = BeginWindow("Setting Window", 2, true);

    /**
     * @brief 主界面渲染逻辑
     * @button Continue Game 继续游戏
     *
     */
    if (ImGui::Button("Continue Game!", BtnSize))
    {
        state = State::RUN;
        glfwSetInputMode(scene->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene->cursorInWindow = true;
    }

    /**
     * @brief 调节Scene中的参数
     *
     */

    // 1. 是否开启垂直同步
    ImGui::Checkbox("VSync", &scene->bVSync);
    glfwSwapInterval(scene->bVSync);
    ImGui::Checkbox("SSR", &scene->ssrOn);

    // 2. 雾效设置
    ImGui::SliderFloat("fogDensity", &scene->fogDensity, 0.0f, 0.1f);
    ImGui::SliderFloat("Sun.x", &scene->SunLightVec.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Sun.y", &scene->SunLightVec.y, -1.0f, 1.0f);
    ImGui::SliderFloat("Sun.z", &scene->SunLightVec.z, -1.0f, 1.0f);
    ImGui::SliderFloat("Sun.front.x", &scene->SunPosition.x, -10.0f, 100.0f);
    ImGui::SliderFloat("Sun.front.y", &scene->SunPosition.y, 80.0f, 300.0f);
    ImGui::SliderFloat("Sun.front.z", &scene->SunPosition.z, -100.0f, 10.0f);
    ImGui::SliderFloat("shadowbias", &scene->shadowBias, .0f, 2.f);

    if (ImGui::Button("Back", BtnSize))
    {
        state = State::WAITING;
    }

    EndWindow();
}

void App::Run()
{
    if (scene->showFPS)
    {
        ImGui::Begin("FPS");
        ImGui::Text("FPS: %.1f", scene->fps);
        ImGui::Text("Selected Block: %d", scene->player->CurBlockID);
        ImGui::End();
    }
}

void App::Waiting()
{
    // 创建一个窗口
    ImVec2 BtnSize = BeginWindow("PAUSE", 3);

    /**
     * @brief 主界面渲染逻辑
     * @button Continue Game 继续游戏
     *
     */
    if (ImGui::Button("Continue Game!", BtnSize))
    {
        state = State::RUN;
        glfwSetInputMode(scene->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        scene->cursorInWindow = true;
    }

    ImGui::Spacing();
    if (ImGui::Button("Settings", BtnSize))
    {
        state = State::SETTING;
    }

    ImGui::Spacing();
    if (ImGui::Button("Save Game", BtnSize))
    {
        // TODO：保存游戏
        /**
         * @brief
         */

        std::ofstream outFile("savegame.txt");
        if (!outFile.is_open())
        {
            std::cerr << "无法打开文件进行写操作" << std::endl;
            return;
        }

        auto map = *scene->map->_map;
        for (const auto &layer : map)
        {
            for (const auto &row : layer)
            {
                for (const auto &cube : row)
                {
                    if (cube != nullptr)
                    {
                        outFile << cube->type() << cube->ID() << std::endl;
                    }
                }
            }
        }

        outFile.close();
        std::cout << "游戏已保存" << std::endl;
    }

    EndWindow();
}

void App::CreatingNewGame()
{
    // 创建一个窗口
    ImGui::Begin("Example Window");

    ImGui::Text("CREATING NEW WORLD! %d", 1);

    if (scene->bMapReady)
    {
        state = State::RUN;
    }

    // ImGui::PopStyleVar();
    ImGui::End();
}

void App::Picture()
{
    ImGui::Begin("Picture Mode");
    ImGui::End();
}

int App::GetSceneWidth()
{
    return scene->imageWidth;
}

int App::GetSceneHeight()
{
    return scene->imageHeight;
}

ImVec2 App::BeginWindow(std::string title, int BtnCount, bool setting)
{
    const char *cstr = title.c_str();
    // 创建一个窗口, 大小和windows窗口一致
    ImGui::SetNextWindowSize(ImVec2(GetSceneWidth(), GetSceneHeight()));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(cstr, NULL, ImGuiWindowFlags_NoResize |            
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
                
    ImGui::Text("FPS: %.1f", scene->fps);

    // 设置无边框
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    // 设置buttons样式
    float ButtonWidth = 500.f;
    float ButtonHeight = 50.f;
    // 设置button之间的间距
    float SpaceBetweenButtons = 30.f;
    float TotalBtnHeight = (ButtonHeight + SpaceBetweenButtons) * BtnCount;
    float startY = (GetSceneHeight() - TotalBtnHeight) / 2.f;
    float startX = (GetSceneWidth() - ButtonWidth) * 0.5f;

    // 设置样式
    ImGuiStyle& style = ImGui::GetStyle();
    style.ItemSpacing = ImVec2(0, SpaceBetweenButtons);
    style.FrameRounding = 10.0f;  // buttons 圆角
    
   
   
    if(!setting)
    {
         // 设置字体大小
        ImGui::SetWindowFontScale(2.f);

        ImGui::SetCursorPosX(startX);
        ImGui::SetCursorPosY(startY);
    }
    else
    {
        ImGui::SetWindowFontScale(1.5f);

        ImGui::SetCursorPosX(180.f);
        ImGui::SetCursorPosY(100.f);
    }
    

    ImGui::BeginGroup();

    return ImVec2(ButtonWidth, ButtonHeight);
}

void App::EndWindow()
{
    ImGui::EndGroup();
    ImGui::PopStyleVar();
    ImGui::SetWindowFontScale(1.f);
    ImGui::End();
}