#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>

#include "Keys.h"

namespace grafyte {
    using inputs::Key;

    enum InputTrigger
    {
        Press,
        Hold,
        Release
    };

    struct InputAction
    {
        Key key;
        InputTrigger trigger;
    };

    class InputManager {
    public:
        static void Init();

        static void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void resetInputs() {
            m_actionPressed.clear();
            m_actionReleased.clear();
            m_keyPressed.clear();
            m_keyReleased.clear();
        }

        static void createAction(const std::string &name, const Key &key, const InputTrigger& trigger) {
            m_inputActions[name] = InputAction{key, trigger};
        }

        static bool isKeyDown(const Key& key) {
            return m_keyDown[key];
        }

        static bool wasKeyPressed(const Key& key) {
            return m_keyPressed[key];
        }

        static bool wasKeyReleased(const Key& key) {
            return m_keyReleased[key];
        }

        static bool isActionActive(const std::string &name);

    private:
        static inline const std::unordered_map<inputs::Key, char> m_logicalKeys = {
            {Key::A, 'A'},
            {Key::B, 'B'},
            {Key::C, 'C'},
            {Key::D, 'D'},
            {Key::E, 'E'},
            {Key::F, 'F'},
            {Key::G, 'G'},
            {Key::H, 'H'},
            {Key::I, 'I'},
            {Key::J, 'J'},
            {Key::K, 'K'},
            {Key::L, 'L'},
            {Key::M, 'M'},
            {Key::N, 'N'},
            {Key::O, 'O'},
            {Key::P, 'P'},
            {Key::Q, 'Q'},
            {Key::R, 'R'},
            {Key::S, 'S'},
            {Key::T, 'T'},
            {Key::U, 'U'},
            {Key::V, 'V'},
            {Key::W, 'W'},
            {Key::X, 'X'},
            {Key::Y, 'Y'},
            {Key::Z, 'Z'},

            {Key::Num0, '0'},
            {Key::Num1, '1'},
            {Key::Num2, '2'},
            {Key::Num3, '3'},
            {Key::Num4, '4'},
            {Key::Num5, '5'},
            {Key::Num6, '6'},
            {Key::Num7, '7'},
            {Key::Num8, '8'},
            {Key::Num9, '9'},
            {Key::Space, ' '},
        };

        static inline std::unordered_map<Key, int> m_keyToGLFW;
        static inline std::unordered_map<int, Key> m_glfwToKey;
        static inline std::unordered_map<std::string, InputAction> m_inputActions;
        static void ensureActionExists(const std::string& name) {
            if (!m_inputActions.contains(name)) throw std::runtime_error("Input action: " + name + " does not exist.");
        };

        static inline std::unordered_map<std::string, bool> m_actionDown;
        static inline std::unordered_map<std::string, bool> m_actionPressed;
        static inline std::unordered_map<std::string, bool> m_actionReleased;

        static inline std::unordered_map<Key, bool> m_keyDown;
        static inline std::unordered_map<Key, bool> m_keyPressed;
        static inline std::unordered_map<Key, bool> m_keyReleased;
    };
}
