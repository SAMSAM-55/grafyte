#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>

#include "Keys.h"

namespace grafyte {
    using inputs::Key;
    using Keys = std::vector<Key>;

    enum InputTrigger
    {
        Press,
        Hold,
        Release
    };

    struct InputAction
    {
        Keys keys;
        InputTrigger trigger{};
    };


    class InputManager {
    public:
        static void Init();

        static void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void resetInputs() {
            m_actionPressed().clear();
            m_actionReleased().clear();
            m_keyPressed().clear();
            m_keyReleased().clear();
        }

        template<typename T>
        static bool getOrFalse(const std::unordered_map<T, bool>& map, const T& key)
        {
            const auto it = map.find(key);
            return it != map.end() ? it->second : false;
        }

        static void createAction(const std::string &name, const Keys &keys, const InputTrigger& trigger) {
            m_inputActions().insert_or_assign(name, InputAction{keys, trigger});
        }

        static bool isKeyDown(const Key& key) {
            return getOrFalse(m_keyDown(), key);
        }

        static bool wasKeyPressed(const Key& key) {
            return getOrFalse(m_keyPressed(), key);
        }

        static bool wasKeyReleased(const Key& key) {
            return getOrFalse(m_keyReleased(), key);
        }

        static bool isActionActive(const std::string &name);

    private:
        static constexpr std::pair<Key, char> m_logicalKeys[] = {
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
        };

        static std::unordered_map<Key, int>& m_keyToGLFW() {
            static auto* map = new std::unordered_map<Key, int>();
            return *map;
        }

        static std::unordered_map<int, Key>& m_glfwToKey() {
            static auto* map = new std::unordered_map<int, Key>();
            return *map;
        }
        static std::unordered_map<std::string, InputAction>& m_inputActions() {
            static auto* map = new std::unordered_map<std::string, InputAction>();
            return *map;
        }
        static void ensureActionExists(const std::string& name) {
            if (!m_inputActions().contains(name)) throw std::runtime_error("Input action: " + name + " does not exist.");
        };

        static std::unordered_map<std::string, bool>& m_actionDown() {
            static auto* map = new std::unordered_map<std::string, bool>();
            return *map;
        }
        static std::unordered_map<std::string, bool>& m_actionPressed() {
            static auto* map = new std::unordered_map<std::string, bool>();
            return *map;
        }
        static std::unordered_map<std::string, bool>& m_actionReleased() {
            static auto* map = new std::unordered_map<std::string, bool>();
            return *map;
        }

        static std::unordered_map<Key, bool>& m_keyDown() {
            static auto* map = new std::unordered_map<Key, bool>();
            return *map;
        }
        static std::unordered_map<Key, bool>& m_keyPressed() {
            static auto* map = new std::unordered_map<Key, bool>();
            return *map;
        }
        static std::unordered_map<Key, bool>& m_keyReleased() {
            static auto* map = new std::unordered_map<Key, bool>();
            return *map;
        }
    };
}
