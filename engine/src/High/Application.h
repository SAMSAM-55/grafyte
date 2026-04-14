#pragma once

#include "glad/glad.h" // required for glfw3

#include "GLFW/glfw3.h"
#include <memory>

#include "Inputs/InputManager.h"
#include "Scene/Scene.h"
#include "Text/TextRenderer.h"
#include "World/World.h"
#include "types.h"

namespace grafyte
{
class Application
{
  public:
    explicit Application(std::string name, std::string font);
    explicit Application(std::string name);
    ~Application();

    int init(int winWidth, int winHeight);
    [[nodiscard]] bool shouldClose() const;
    void quit();

    [[nodiscard]] double getNow() const
    {
        return m_Now;
    }
    [[nodiscard]] double getDeltaTime() const
    {
        return m_DeltaTime;
    }

    void render();

    static bool isKeyDown(const Key &key)
    {
        return InputManager::isKeyDown(key);
    };
    static bool wasKeyPressed(const Key &key)
    {
        return InputManager::wasKeyPressed(key);
    };
    static bool wasKeyReleased(const Key &key)
    {
        return InputManager::wasKeyReleased(key);
    };

    static bool isActionActive(const std::string &name)
    {
        return InputManager::isActionActive(name);
    };
    static void createInputAction(const std::string &name, const Keys &keys, const InputTrigger &trigger)
    {
        InputManager::createAction(name, trigger, keys);
    }

    void setClearColor(float r, float g, float b, float a);

    std::shared_ptr<Scene> makeNewScene();
    void setActiveScene(const types::SceneId &id);
    void freezeScenes();
    [[nodiscard]] std::shared_ptr<Scene> getActiveScene() const
    {
        return scene;
    }
    std::shared_ptr<UIManager> makeNewUI();

    void endFrame() const;

    static Application *sAppInstance;
    std::shared_ptr<WorldContext> ctx;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<UIManager> ui;

  private:
    void beginFrame() const;
    void computeCamera() const;

    const std::string m_Name;
    std::unique_ptr<TextRenderer> m_TextRenderer;
    std::unordered_map<types::SceneId, std::shared_ptr<Scene>> m_Scenes;
    types::SceneId m_NextSceneId = 0;

    GLFWwindow *m_Window;
    int m_WinWidth, m_WinHeight;
    types::Color4 m_ClearColor;

    std::string m_Font;
    int m_NextTextId = 0;

    double m_Now = 0.0f;
    double m_LastFrame = 0.0f;
    double m_DeltaTime = 0.0f;
};
} // namespace grafyte
