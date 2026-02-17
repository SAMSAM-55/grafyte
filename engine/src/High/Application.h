#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Intermediate/Renderer.h"

namespace grafyte 
{
	class Application
	{
	public:
		struct Color4
		{
			float x, y, z, w;
		};

		explicit Application(std::string name);
		~Application();

		void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);
		int init(int winWidth, int winHeight);

		void quit();

		[[nodiscard]] bool shouldClose() const;

		[[nodiscard]] double getNow() const {return m_now;}

		[[nodiscard]] double getDeltaTime() const {return m_deltaTime;}

		void render();
		[[nodiscard]] int getCurrentInput() const;
		[[nodiscard]] inline bool isKeyDown(const char16_t& key) const {return glfwGetKey(m_window, key) == GLFW_PRESS;};
		void setClearColor(float r, float g, float b, float a);

		void useRenderer(const Renderer &renderer);

	private:
		GLFWwindow* m_window;
		Renderer m_renderer;
		int m_winWidth, m_winHeight;
		double m_now = 0.0f;
		double m_lastFrame = 0.0f;
		double m_deltaTime = 0.0f;
		Color4 m_clearColor;
		const std::string m_name;
		int m_currentInput;
		glm::mat4 m_MVP;
	};
}