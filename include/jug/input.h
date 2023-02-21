#pragma once

#include "glm/glm.hpp"

#include "keyCodes.h"

namespace Jug
{
    class Input
    {
    public:
        static bool isKeyDown(KeyCode keycode);
        static bool isMouseButtonDown(MouseButton button);

        static glm::vec2 getMousePosition();

        static void setCursorMode(CursorMode mode);
    };
} // namespace Jug
