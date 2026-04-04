#include "Application.h"

#include <iostream>
#include <utility>

#include <glm/ext/matrix_clip_space.hpp>

namespace grafyte
{
    Application* Application::s_appInstance = nullptr;

    Application::Application(std::string  name, std::string font)
        : scene(nullptr), m_name(std::move(name)), m_textRenderer(nullptr),
          m_window(nullptr), m_winWidth(0), m_winHeight(0), m_clearColor({0, 0, 0, 0}),
          m_font(std::move(font))
    {
    }

    Application::Application(std::string name)
        : Application(std::move(name), "res/fonts/Inter_Medium.ttf")
    {
    }

    Application::~Application()
    = default;

    int Application::init(const int winWidth, const int winHeight) {
        std::cout << "[init] glfwInit" << std::endl;
        if (!glfwInit()) {
            return -1;
        }

        glfwSetErrorCallback([](int error, const char* description) {
            fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
        });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_winHeight = winHeight;
        m_winWidth = winWidth;

        std::cout << "[init] createWindow" << std::endl;
        m_window = glfwCreateWindow(winWidth, winHeight, m_name.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(m_window);

        std::cout << "[init] gladLoad" << std::endl;
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Could not load glad.");
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        s_appInstance = this;
        InputManager::Init();
        ctx = std::make_unique<WorldContext>();
        ctx->init();
        std::cout << "[init] TextRenderer" << std::endl;
        m_textRenderer = std::make_unique<TextRenderer>(m_font, 32);
        std::cout << "[init] SetDpi" << std::endl;
        m_textRenderer->SetDpi({96.0f, 96.0f});
        glfwSetKeyCallback(m_window, InputManager::on_key);
        glfwSwapInterval(1);

        std::cout << "[init] done" << std::endl;
        return 0;
	}

    void Application::quit() {
        if (!m_window) return;

        // Clear scenes and managers while context is definitely alive
        if (scene) scene->clear();
        ctx->meshes.clear();
        ctx->materials.clear();
        ctx.reset();

        m_textRenderer.reset(); // Specifically destroy the unique_ptr here

        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
    }

    bool Application::shouldClose() const {
        return glfwWindowShouldClose(m_window);
	}

    void Application::render() {
        BeginFrame();

        m_now = glfwGetTime();
        m_deltaTime = m_now - m_lastFrame;
        m_lastFrame = m_now;

        // std::cout << "[Application](Render): Frame started. DeltaTime: " << m_deltaTime << std::endl;

        glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        grafyte::Renderer::Clear();

        glfwGetFramebufferSize(m_window, &m_winWidth, &m_winHeight);
        glViewport(0, 0, m_winWidth, m_winHeight);

        // Render
        computeProjection();
        std::vector<types::TextData> texts;
        auto& transforms = scene->GetTransforms();
        auto& colors = scene->GetColors();
        const auto& items = scene->getBatchedRenderList();
        scene->GetTextRenderList(texts);

        // if (!texts.empty()) {
        //     std::cout << "[Application](render): texts.size() = " << texts.size() << std::endl;
        //     std::cout << "[Application](render): winDim=(" << m_winWidth << ", " << m_winHeight << ")" << std::endl;
        // }

        ctx->renderer.Render(items, transforms, colors, ctx->camera);
        m_textRenderer->Render(texts, &ctx->camera);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        /* Poll for and process events */
        glfwPollEvents();

        // std::cout << "[Application](Render): Frame completed." << std::endl;
    }

    void Application::setClearColor(const float r, const float g, const float b, const float a) {
        m_clearColor = types::Color4(r, g, b, a);
	}

    Scene& Application::makeNewScene() {
        if (scene) {
            scene->clear();
        }

        // Ensure the context exists and is initialized
        if (!ctx) {
            ctx = std::make_unique<WorldContext>();
            ctx->init();
        } else {
            ctx->meshes.clear();
            ctx->materials.clear();
            ctx->init(); // Re-init default textures if needed
        }

        scene = std::make_unique<Scene>(ctx.get());
        return *scene;
    }

    void Application::BeginFrame() {
        InputManager::resetInputs();
    }

    void Application::computeProjection() {
        const double aspect = static_cast<double>(m_winWidth) / m_winHeight;
        const float worldHeight = 200.0f;
        const float worldWidth = worldHeight * static_cast<float>(aspect);

        ctx->camera.projection = glm::ortho(-worldWidth / 2.0f, worldWidth / 2.0f, -worldHeight / 2.0f, worldHeight / 2.0f, -1.0f, 1.0f);
    }
}






