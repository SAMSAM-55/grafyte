#include "InputManager.h"
#include <GLFW/glfw3.h>

#include <cctype>
#include <cstring>

namespace grafyte
{
void InputManager::init()
{
    mKeyToGlfw().clear();
    mGlfwToKey().clear();
    for (int glfwKey = GLFW_KEY_SPACE; glfwKey <= GLFW_KEY_LAST; glfwKey++)
    {
        const int scancode = glfwGetKeyScancode(glfwKey);
        if (scancode == -1)
            continue;

        const char *name = glfwGetKeyName(glfwKey, scancode);
        if (!name)
            continue;
        if (const size_t nameLen = std::strlen(name); nameLen != 1)
            continue;

        const char produced = static_cast<char>(std::tolower(static_cast<unsigned char>(name[0])));

        for (const auto &[key, expected] : m_LogicalKeys)
        {
            if (std::tolower(static_cast<unsigned char>(expected)) == produced)
            {
                mKeyToGlfw()[key] = glfwKey;
                mGlfwToKey()[glfwKey] = key;
                break;
            }
        }
    }

    mKeyToGlfw()[Key::Left] = GLFW_KEY_LEFT;
    mGlfwToKey()[GLFW_KEY_LEFT] = Key::Left;

    mKeyToGlfw()[Key::Right] = GLFW_KEY_RIGHT;
    mGlfwToKey()[GLFW_KEY_RIGHT] = Key::Right;

    mKeyToGlfw()[Key::Up] = GLFW_KEY_UP;
    mGlfwToKey()[GLFW_KEY_UP] = Key::Up;

    mKeyToGlfw()[Key::Down] = GLFW_KEY_DOWN;
    mGlfwToKey()[GLFW_KEY_DOWN] = Key::Down;

    mKeyToGlfw()[Key::LeftShift] = GLFW_KEY_LEFT_SHIFT;
    mGlfwToKey()[GLFW_KEY_LEFT_SHIFT] = Key::LeftShift;

    mKeyToGlfw()[Key::RightShift] = GLFW_KEY_RIGHT_SHIFT;
    mGlfwToKey()[GLFW_KEY_RIGHT_SHIFT] = Key::RightShift;

    mKeyToGlfw()[Key::Space] = GLFW_KEY_SPACE;
    mGlfwToKey()[GLFW_KEY_SPACE] = Key::Space;
}

void InputManager::onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (mGlfwToKey().contains(key))
        {
            const Key k = mGlfwToKey()[key];
            mKeyDown().insert_or_assign(k, true);
            mKeyPressed().insert_or_assign(k, true);
        }

        for (const auto &[name, inputAction] : mInputActions())
        {
            for (const auto &inputKey : inputAction.keys)
            {
                if (mKeyToGlfw().at(inputKey) == key)
                {
                    mActionDown().insert_or_assign(name, true);
                    mActionPressed().insert_or_assign(name, true);
                }
            }
        }
    }

    if (action == GLFW_RELEASE)
    {
        if (mGlfwToKey().contains(key))
        {
            const Key k = mGlfwToKey()[key];
            mKeyDown().insert_or_assign(k, false);
            mKeyReleased().insert_or_assign(k, true);
        }

        for (const auto &[name, inputAction] : mInputActions())
        {
            for (const auto &inputKey : inputAction.keys)
            {
                if (mKeyToGlfw().at(inputKey) == key)
                {
                    mActionDown().insert_or_assign(name, false);
                    mActionReleased().insert_or_assign(name, true);
                }
            }
        }
    }
}

bool InputManager::isActionActive(const std::string &name)
{
    ensureActionExists(name);
    switch (const auto &[_, trigger] = mInputActions().at(name); trigger)
    {
    case Press:
        return getOrFalse(mActionPressed(), name);
    case Hold:
        return getOrFalse(mActionDown(), name);
    case Release:
        return getOrFalse(mActionReleased(), name);
    default:
        return false;
    }
}
} // namespace grafyte
