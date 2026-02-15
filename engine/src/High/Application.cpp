#include "Application.h"

#include <iostream>
#include <utility>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


namespace Grafyte 
{
    Application::Application(std::string name)
        :m_winWidth(0), m_winHeight(0), m_clearColor({0, 0, 0, 0}), m_name(std::move(name)), m_currentInput(-1)
    {
        const glm::mat4 proj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
        constexpr glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        constexpr auto model = glm::mat4(1.0f);

        this->m_MVP = proj * view * model;
    }
    Application::~Application()
    = default;

    void Application::on_key(GLFWwindow* window, const int key, int scancode, const int action, int mods)
    {
        if (action == GLFW_PRESS)
            m_currentInput = key;
        if (action == GLFW_RELEASE)
            m_currentInput = -1;
    }

    static Application* g_appInstance = nullptr;

    static void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
    {
        if (g_appInstance)
            g_appInstance->on_key(window, key, scancode, action, mods);
    }

    int Application::init(const int winWidth, const int winHeight)
	{
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

        if (glewInit() != GLEW_OK)
            std::cout << "Error!" << std::endl;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        g_appInstance = this;
        glfwSetKeyCallback(m_window, key_callback);

        return 0;
	}

    void Application::quit() {
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
        m_now = glfwGetTime();
        m_deltaTime = m_now - m_lastFrame;
        m_lastFrame = m_now;

        glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        Grafyte::Renderer::Clear();

        glfwGetFramebufferSize(m_window, &m_winWidth, &m_winHeight);
        glViewport(0, 0, m_winWidth, m_winHeight);

        const double aspect = static_cast<double>(m_winWidth) / m_winHeight;
        const float halfWorldWidth = 100.0f * aspect;

        const glm::mat4 proj = glm::ortho(-halfWorldWidth, halfWorldWidth, -100.0f, 100.0f, -1.0f, 1.0f);
        constexpr glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        constexpr auto model = glm::mat4(1.0f);

        this->m_MVP = proj * view * model;

        m_renderer.Render(m_MVP);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        /* Poll for and process events */
        glfwPollEvents();

    }
	int Application::getCurrentInput() const
	{
		return m_currentInput;
	}
	void Application::setClearColor(const float r, const float g, const float b, const float a)
	{
        m_clearColor = Color4(r, g, b, a);
	}

    void Application::useRenderer(const Grafyte::Renderer &renderer) {
        this->m_renderer = renderer;
    }
}
