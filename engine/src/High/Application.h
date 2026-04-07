#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <memory>

#include "Inputs/InputManager.h"
#include "Scene/Scene.h"
#include "Text/TextRenderer.h"
#include "World/World.h"

namespace grafyte
{
	class Application
	{
	public:
		explicit Application(std::string  name, std::string font);
		explicit Application(std::string  name);
		~Application();

		int init(int winWidth, int winHeight);
		[[nodiscard]] bool shouldClose() const;
		void quit();

		[[nodiscard]] double getNow() const {return m_now;}
		[[nodiscard]] double getDeltaTime() const {return m_deltaTime;}

		void render();

		static bool isKeyDown(const Key& key) {return InputManager::isKeyDown(key);};
		static bool wasKeyPressed(const Key& key) {return InputManager::wasKeyPressed(key);};
		static bool wasKeyReleased(const Key& key) {return InputManager::wasKeyReleased(key);};

		static bool isActionActive(const std::string& name) {return InputManager::isActionActive(name);};
		static void createInputAction(const std::string &name, const Keys &keys, const InputTrigger& trigger) {
			InputManager::createAction(name, keys, trigger);
		}
		
		void setClearColor(float r, float g, float b, float a);

		std::shared_ptr<Scene> makeNewScene();

		void EndFrame() const;

		static Application* s_appInstance;
		std::shared_ptr<WorldContext> ctx;
		std::shared_ptr<Scene> scene;
	private:
		void BeginFrame() const;
		void computeProjection() const;

		const std::string m_name;
		std::unique_ptr<TextRenderer> m_textRenderer;

		GLFWwindow* m_window;
		int m_winWidth, m_winHeight;
		types::Color4 m_clearColor;

		std::string m_font;
		int m_nextTextId = 0;

		double m_now = 0.0f;
		double m_lastFrame = 0.0f;
		double m_deltaTime = 0.0f;

	};
}
