#include "jug/application.h"
#include "jug/input.h"

namespace Jug
{
    bool Input::isKeyDown(KeyCode keycode)
    {
        auto window = Application::getInstance()->getWindow();
        int state = glfwGetKey(window, (int)keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonDown(MouseButton button)
    {
        auto window = Application::getInstance()->getWindow();
        int state = glfwGetMouseButton(window, (int)button);
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::getMousePosition()
    {
        auto window = Application::getInstance()->getWindow();
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {(float)x, (float)y};
    }

    void Input::setCursorMode(CursorMode mode)
    {
        auto window = Application::getInstance()->getWindow();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
    }
}