#include "InputManager.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <cctype>

namespace grafyte {
    void InputManager::Init() {
        m_keyToGLFW().clear();
        m_glfwToKey().clear();
        for (int glfwKey = GLFW_KEY_SPACE; glfwKey <= GLFW_KEY_LAST; glfwKey++) {
            const int scancode = glfwGetKeyScancode(glfwKey);
            if (scancode == -1) continue;

            const char* name = glfwGetKeyName(glfwKey, scancode);
            if (!name) continue;
            if (const size_t nameLen = std::strlen(name); nameLen != 1) continue;

            const char produced = static_cast<char>(std::tolower(static_cast<unsigned char>(name[0])));

            for (const auto&[key, expected]: m_logicalKeys) {
                if (std::tolower(static_cast<unsigned char>(expected)) == produced) {
                    m_keyToGLFW()[key] = glfwKey;
                    m_glfwToKey()[glfwKey] = key;
                    break;
                }
            }
        }

        m_keyToGLFW()[Key::Left] = GLFW_KEY_LEFT;
        m_glfwToKey()[GLFW_KEY_LEFT] = Key::Left;

        m_keyToGLFW()[Key::Right] = GLFW_KEY_RIGHT;
        m_glfwToKey()[GLFW_KEY_RIGHT] = Key::Right;

        m_keyToGLFW()[Key::Up] = GLFW_KEY_UP;
        m_glfwToKey()[GLFW_KEY_UP] = Key::Up;

        m_keyToGLFW()[Key::Down] = GLFW_KEY_DOWN;
        m_glfwToKey()[GLFW_KEY_DOWN] = Key::Down;

        m_keyToGLFW()[Key::LeftShift] = GLFW_KEY_LEFT_SHIFT;
        m_glfwToKey()[GLFW_KEY_LEFT_SHIFT] = Key::LeftShift;

        m_keyToGLFW()[Key::RightShift] = GLFW_KEY_RIGHT_SHIFT;
        m_glfwToKey()[GLFW_KEY_RIGHT_SHIFT] = Key::RightShift;

        m_keyToGLFW()[Key::Space] = GLFW_KEY_SPACE;
        m_glfwToKey()[GLFW_KEY_SPACE] = Key::Space;
    }

    void InputManager::on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            if (m_glfwToKey().contains(key)) {
                const Key k = m_glfwToKey()[key];
                m_keyDown().insert_or_assign(k, true);
                m_keyPressed().insert_or_assign(k, true);
            }

            for (const auto&[name, inputAction]: m_inputActions()) {
                for (const auto& inputKey: inputAction.keys) {
                    if (m_keyToGLFW().at(inputKey) == key) {
                        m_actionDown().insert_or_assign(name, true);
                        m_actionPressed().insert_or_assign(name, true);
                    }
                }
            }
        }

        if (action == GLFW_RELEASE) {
            if (m_glfwToKey().contains(key)) {
                const Key k = m_glfwToKey()[key];
                m_keyDown().insert_or_assign(k, false);
                m_keyReleased().insert_or_assign(k, true);
            }

            for (const auto&[name, inputAction]: m_inputActions()) {
                for (const auto& inputKey: inputAction.keys) {
                    if (m_keyToGLFW().at(inputKey) == key) {
                        m_actionDown().insert_or_assign(name, false);
                        m_actionReleased().insert_or_assign(name, true);
                    }
                }
            }
        }
    }

    bool InputManager::isActionActive(const std::string& name)
    {
        ensureActionExists(name);
        switch (const auto&[_, trigger] = m_inputActions().at(name); trigger)
        {
            case Press: return getOrFalse(m_actionPressed(),name);
            case Hold: return getOrFalse(m_actionDown(),name);
            case Release: return getOrFalse(m_actionReleased(),name);
            default: return false;
        }
    }
}
