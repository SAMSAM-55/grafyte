#pragma once

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Keys.h"

namespace grafyte
{
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

class InputManager
{
  public:
    static void init();

    static void onKey(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void resetInputs()
    {
        mActionPressed().clear();
        mActionReleased().clear();
        mKeyPressed().clear();
        mKeyReleased().clear();
    }

    template <typename T> static bool getOrFalse(const std::unordered_map<T, bool> &map, const T &key)
    {
        const auto it = map.find(key);
        return it != map.end() ? it->second : false;
    }

    static void createAction(const std::string &name, const Keys &keys, const InputTrigger &trigger)
    {
        mInputActions().insert_or_assign(name, InputAction{keys, trigger});
    }

    static bool isKeyDown(const Key &key)
    {
        return getOrFalse(mKeyDown(), key);
    }

    static bool wasKeyPressed(const Key &key)
    {
        return getOrFalse(mKeyPressed(), key);
    }

    static bool wasKeyReleased(const Key &key)
    {
        return getOrFalse(mKeyReleased(), key);
    }

    static bool isActionActive(const std::string &name);

  private:
    static constexpr std::pair<Key, char> m_LogicalKeys[] = {
        {Key::A, 'A'},    {Key::B, 'B'},    {Key::C, 'C'},    {Key::D, 'D'},    {Key::E, 'E'},    {Key::F, 'F'},
        {Key::G, 'G'},    {Key::H, 'H'},    {Key::I, 'I'},    {Key::J, 'J'},    {Key::K, 'K'},    {Key::L, 'L'},
        {Key::M, 'M'},    {Key::N, 'N'},    {Key::O, 'O'},    {Key::P, 'P'},    {Key::Q, 'Q'},    {Key::R, 'R'},
        {Key::S, 'S'},    {Key::T, 'T'},    {Key::U, 'U'},    {Key::V, 'V'},    {Key::W, 'W'},    {Key::X, 'X'},
        {Key::Y, 'Y'},    {Key::Z, 'Z'},

        {Key::Num0, '0'}, {Key::Num1, '1'}, {Key::Num2, '2'}, {Key::Num3, '3'}, {Key::Num4, '4'}, {Key::Num5, '5'},
        {Key::Num6, '6'}, {Key::Num7, '7'}, {Key::Num8, '8'}, {Key::Num9, '9'},
    };

    static std::unordered_map<Key, int> &mKeyToGlfw()
    {
        static auto *s_Map = new std::unordered_map<Key, int>();
        return *s_Map;
    }

    static std::unordered_map<int, Key> &mGlfwToKey()
    {
        static auto *s_Map = new std::unordered_map<int, Key>();
        return *s_Map;
    }
    static std::unordered_map<std::string, InputAction> &mInputActions()
    {
        static auto *s_Map = new std::unordered_map<std::string, InputAction>();
        return *s_Map;
    }
    static void ensureActionExists(const std::string &name)
    {
        if (!mInputActions().contains(name))
            throw std::runtime_error("Input action: " + name + " does not exist.");
    };

    static std::unordered_map<std::string, bool> &mActionDown()
    {
        static auto *s_Map = new std::unordered_map<std::string, bool>();
        return *s_Map;
    }
    static std::unordered_map<std::string, bool> &mActionPressed()
    {
        static auto *s_Map = new std::unordered_map<std::string, bool>();
        return *s_Map;
    }
    static std::unordered_map<std::string, bool> &mActionReleased()
    {
        static auto *s_Map = new std::unordered_map<std::string, bool>();
        return *s_Map;
    }

    static std::unordered_map<Key, bool> &mKeyDown()
    {
        static auto *s_Map = new std::unordered_map<Key, bool>();
        return *s_Map;
    }
    static std::unordered_map<Key, bool> &mKeyPressed()
    {
        static auto *s_Map = new std::unordered_map<Key, bool>();
        return *s_Map;
    }
    static std::unordered_map<Key, bool> &mKeyReleased()
    {
        static auto *s_Map = new std::unordered_map<Key, bool>();
        return *s_Map;
    }
};
} // namespace grafyte
