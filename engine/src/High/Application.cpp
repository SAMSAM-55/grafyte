#include "Application.h"

#include <iostream>
#include <ranges>
#include <utility>

#include <glm/ext/matrix_clip_space.hpp>

namespace grafyte
{
    Application* Application::s_appInstance = nullptr;

    Application::Application(std::string  name, std::string font)
        : scene(nullptr), m_name(std::move(name)), m_window(nullptr), m_winWidth(0), m_winHeight(0),
          m_clearColor({0, 0, 0, 0}), m_font(std::move(font)), m_texts({}) {
    }

    Application::~Application()
    = default;

    int Application::init(const int winWidth, const int winHeight) {
        if (!glfwInit())
            return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_winHeight = winHeight;
        m_winWidth = winWidth;

        m_window = glfwCreateWindow(winWidth, winHeight, m_name.c_str(), nullptr, nullptr);
        glfwSwapInterval(1);
        if (!m_window)
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw std::runtime_error("Could not load glad.");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        s_appInstance = this;
        InputManager::Init();
        glfwSetKeyCallback(m_window, InputManager::on_key);
        m_textRenderer = std::make_unique<TextRenderer>(m_font, 32);

        return 0;
	}

    void Application::quit() {
        ctx.meshes.clear();
        ctx.materials.clear();

        m_textRenderer.reset();
        scene->clear();

        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
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
        std::vector<types::DrawItem> items;
        scene->buildRenderList(items);
        ctx.renderer.Render(items, ctx.camera);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        /* Poll for and process events */
        glfwPollEvents();

        // std::cout << "[Application](Render): Frame completed." << std::endl;
    }

    /*
    void Application::drawTexts() const {
        for (const auto &[text, scale, posX, posY]: m_texts | std::views::values) {
            const float textWidth = m_textRenderer->MeasureTextWidth(text, scale);
            const float posXFinal = posX - textWidth / 2;

            m_textRenderer->DrawText(text,
                posXFinal, posY+scale,
                scale,
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                m_MVP);
        }
    }
    */

    void Application::setClearColor(const float r, const float g, const float b, const float a) {
        m_clearColor = types::Color4(r, g, b, a);
	}

    Scene& Application::makeNewScene()
    {
        ctx.meshes.clear();
        ctx.materials.clear();
        if (scene)
            scene->clear();
        scene = std::make_unique<Scene>(&ctx);
        return *scene;
    }

    void Application::BeginFrame() {
        InputManager::resetInputs();
    }

    void Application::computeProjection() {
        const double aspect = static_cast<double>(m_winWidth) / m_winHeight;
        const float halfWorldWidth = 100.0f * aspect;

        ctx.camera.projection = glm::ortho(-halfWorldWidth, halfWorldWidth, -100.0f, 100.0f, -1.0f, 1.0f);
    }
}
