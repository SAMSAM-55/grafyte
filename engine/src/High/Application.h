#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <memory>

#include "Inputs/InputManager.h"
#include "Scene/Scene.h"
#include "Text/TextRenderer.h"
#include "Text/Text.h"
#include "World/World.h"

namespace grafyte
{
	class Application
	{
	public:


		explicit Application(std::string  name, std::string font);
		~Application();

		int init(int winWidth, int winHeight);
		[[nodiscard]] bool shouldClose() const;
		void quit();

		[[nodiscard]] double getNow() const {return m_now;}
		[[nodiscard]] double getDeltaTime() const {return m_deltaTime;}

		void render();
		// void drawTexts() const;

		// int addText(const std::string &text, const float &scale, const float &pos_x, const float &pos_y) {
		// 	const int id = m_nextTextId++;
		// 	m_texts.emplace(id, Text{text, scale, pos_x, pos_y});
		// 	return id;
		// }

		// void removeText(const int& id) {
		// 	m_texts.erase(id);
		// }

		static bool isKeyDown(const Key& key) {return InputManager::isKeyDown(key);};
		static bool wasKeyPressed(const Key& key) {return InputManager::wasKeyPressed(key);};
		static bool wasKeyReleased(const Key& key) {return InputManager::wasKeyReleased(key);};

		static bool isActionActive(const std::string& name) {return InputManager::isActionActive(name);};
		static void createInputAction(const std::string &name, const Key &key, const InputTrigger& trigger) {
			InputManager::createAction(name, key, trigger);
		}
		
		void setClearColor(float r, float g, float b, float a);

		Scene& makeNewScene();

		static Application* s_appInstance;
		WorldContext ctx;
		std::unique_ptr<Scene> scene;
	private:
		static void BeginFrame();
		void computeProjection();

		const std::string m_name;

		GLFWwindow* m_window;
		int m_winWidth, m_winHeight;
		types::Color4 m_clearColor;

		std::string m_font;
		std::unique_ptr<TextRenderer> m_textRenderer;
		std::unordered_map<int, Text> m_texts;
		int m_nextTextId = 0;

		double m_now = 0.0f;
		double m_lastFrame = 0.0f;
		double m_deltaTime = 0.0f;

	};
}
