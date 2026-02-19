#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <memory>

#include "Inputs/InputManager.h"
#include "Intermediate/Renderer.h"
#include "Text/TextRenderer.h"
#include "Text/Text.h"

namespace grafyte
{
	class Application
	{
	public:
		struct Color4
		{
			float x, y, z, w;
		};

		explicit Application(std::string  name, std::string font);
		~Application();

		static void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);
		int init(int winWidth, int winHeight);

		void quit();

		[[nodiscard]] bool shouldClose() const;

		[[nodiscard]] double getNow() const {return m_now;}

		[[nodiscard]] double getDeltaTime() const {return m_deltaTime;}

		void render();
		void drawTexts() const;

		int addText(const std::string &text, const float &scale, const float &pos_x, const float &pos_y) {
			const int id = m_nextTextId++;
			m_texts.emplace(id, Text{text, scale, pos_x, pos_y});
			return id;
		}

		void removeText(const int& id) {
			m_texts.erase(id);
		}

		static bool isKeyDown(const Key& key) {return InputManager::isKeyDown(key);};
		static bool wasKeyPressed(const Key& key) {return InputManager::wasKeyPressed(key);};
		static bool wasKeyReleased(const Key& key) {return InputManager::wasKeyReleased(key);};
		
		static bool isActionDown(const std::string& name) {return InputManager::isActionDown(name);};
		static bool wasActionPressed(const std::string& name) {return InputManager::wasActionPressed(name);};
		static bool wasActionReleased(const std::string& name) {return InputManager::wasActionReleased(name);};
		static void createInputAction(const std::string &name, const Key &key) {
			InputManager::createAction(name, key);
		}
		
		void setClearColor(float r, float g, float b, float a);

		void useRenderer(const Renderer &renderer);

		static Application* g_appInstance;
		static void BeginFrame();
	private:
		const std::string m_name;

		GLFWwindow* m_window;
		int m_winWidth, m_winHeight;
		Renderer m_renderer;
		Color4 m_clearColor;

		std::string m_font;
		std::unique_ptr<TextRenderer> m_textRenderer;
		std::unordered_map<int, Text> m_texts;
		int m_nextTextId = 0;

		double m_now = 0.0f;
		double m_lastFrame = 0.0f;
		double m_deltaTime = 0.0f;

		glm::mat4 m_MVP{};
	};
}
