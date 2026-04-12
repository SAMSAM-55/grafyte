#include "Application.h"

#include <iostream>
#include <utility>

#include "GlContextState.h"
#include "macros.hpp"

namespace grafyte
{
Application *Application::sAppInstance = nullptr;

Application::Application(std::string name, std::string font)
    : scene(nullptr), ui(nullptr), m_Name(std::move(name)), m_TextRenderer(nullptr), m_Window(nullptr), m_WinWidth(0),
      m_WinHeight(0), m_ClearColor({0, 0, 0, 0}), m_Font(std::move(font))
{
}

Application::Application(std::string name) : Application(std::move(name), "res/fonts/Inter_Medium.ttf")
{
}

Application::~Application()
{
    quit();
}

int Application::init(const int winWidth, const int winHeight)
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwSetErrorCallback(
        [](int error, const char *description) { fprintf(stderr, "GLFW Error (%d): %s\n", error, description); });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_WinHeight = winHeight;
    m_WinWidth = winWidth;

    m_Window = glfwCreateWindow(winWidth, winHeight, m_Name.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_Window);
    glContextAlive() = true;

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Could not load glad.");
    }

    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glBlendEquation(GL_FUNC_ADD));

    sAppInstance = this;
    InputManager::init();
    ctx = std::make_shared<WorldContext>();
    ctx->init();
    m_TextRenderer = std::make_unique<TextRenderer>(m_Font, 64);
    m_TextRenderer->setDpi({96.0f, 96.0f});
    glfwSetKeyCallback(m_Window, InputManager::onKey);
    glfwSwapInterval(1);

    return 0;
}

void Application::quit()
{
    if (!m_Window)
    {
        sAppInstance = nullptr;
        return;
    }

    if (scene)
        scene->clear();
    scene.reset();
    if (ctx)
    {
        ctx->meshes.clear();
        ctx->materials.clear();
        ctx->renderer.clear();
        ctx.reset();
    }

    m_TextRenderer.reset();

    glContextAlive() = false;
    glfwDestroyWindow(m_Window);
    m_Window = nullptr;
    glfwTerminate();
    sAppInstance = nullptr;

    // std::quick_exit(0);
}

bool Application::shouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void Application::render()
{
    beginFrame();

    m_Now = glfwGetTime();
    m_DeltaTime = m_Now - m_LastFrame;
    m_LastFrame = m_Now;

    GL_CALL(glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w));
    Renderer::clearScreen();

    glfwGetFramebufferSize(m_Window, &m_WinWidth, &m_WinHeight);
    GL_CALL(glViewport(0, 0, m_WinWidth, m_WinHeight));

    std::vector<types::TextData> textObjects;
    std::vector<ui::text::Text> texts;

    if (scene)
    {
        computeCamera();

        const auto &transforms = scene->getTransforms();
        const auto &colors = scene->getColors();
        const auto &items = scene->getBatchedRenderList();
        scene->getTextRenderList(textObjects);

        ctx->renderer.render(items, transforms, colors, ctx->camera);
    }

    if (ui)
    {
        ui->getTexts(texts);
    }

    m_TextRenderer->render(textObjects, texts, scene ? &ctx->camera : nullptr,
                           {static_cast<float>(m_WinWidth), static_cast<float>(m_WinHeight)});

    endFrame();
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Application::setClearColor(const float r, const float g, const float b, const float a)
{
    m_ClearColor = types::Color4(r, g, b, a);
}

std::shared_ptr<Scene> Application::makeNewScene()
{
    if (scene)
        scene->clear();

    // Ensure the context exists and is initialized
    if (!ctx)
    {
        ctx = std::make_shared<WorldContext>();
        ctx->init();
    }
    else
    {
        ctx->meshes.clear();
        ctx->materials.clear();
        ctx->init(); // Re-init default textures if needed
    }

    scene = std::make_shared<Scene>(ctx);
    return scene;
}

std::shared_ptr<UIManager> Application::makeNewUI()
{
    if (ui)
        ui->clear();

    if (!ctx)
    {
        ctx = std::make_shared<WorldContext>();
        ctx->init();
    }
    else
    {
        ctx->meshes.clear();
        ctx->materials.clear();
        ctx->init(); // Re-init default textures if needed
    }

    ui = std::make_shared<UIManager>(ctx);
    return ui;
}

void Application::endFrame() const
{
}

void Application::beginFrame() const
{
    InputManager::resetInputs();
    ctx->collisions.reset();
    if (scene)
        ctx->collisions.rebuildGrid(*scene);
}

void Application::computeCamera() const
{
    if (!scene)
        return;

    const double aspect = static_cast<double>(m_WinWidth) / m_WinHeight;
    constexpr float worldHeight = 200.0f;
    const float worldWidth = worldHeight * static_cast<float>(aspect);

    scene->computeCamera(worldWidth, worldHeight, static_cast<float>(m_DeltaTime));
}
} // namespace grafyte
