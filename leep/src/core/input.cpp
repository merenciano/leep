#include "input.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "tools/imgui-tools.h"

#include <GLFW/glfw3.h>

namespace leep
{
    struct Input::InputData
    {
        GLFWwindow *window;
        InputState current;
        InputState previous;
    };

    static int32_t ToGLFW(Button b)
    {
        switch (b)
        {
            case Button::UP: return GLFW_KEY_W;
            case Button::DOWN: return GLFW_KEY_S;
            case Button::LEFT: return GLFW_KEY_A;
            case Button::RIGHT: return GLFW_KEY_D;
            case Button::B1: return GLFW_KEY_SPACE;
            case Button::B2: return GLFW_KEY_Q;
            case Button::B3: return GLFW_KEY_E;
            case Button::B4: return GLFW_KEY_LEFT_SHIFT;
            case Button::MOUSE_LEFT: return GLFW_MOUSE_BUTTON_1;
            case Button::MOUSE_RIGHT: return GLFW_MOUSE_BUTTON_2;
            default: LEEP_CORE_ERROR("Invalid button pressed.\n"); return GLFW_INVALID_ENUM;
        }
    }

    static void ScrollCallback(GLFWwindow *window, double x_offset, double y_offset)
    {
		if (GM.ui_tools().wantMouse())
			return;
        GM.input().set_scroll((float)y_offset);
    }

    Input::Input()
    {

    }

    Input::~Input()
    {
        // The lifetime of this class is the same as the entire program,
        // so I'm not going to slow down the closing process deleting pointers.
#ifdef LEEP_DEBUG
        GM.memory().generalFree(data_);
#endif
    }

    void Input::init(void *window)
    {
        if (window == nullptr)
            return;
        
        if (data_)
            return;

        data_ = GM.memory().generalAllocT<InputData>(1);
        data_->window = (GLFWwindow*)window;
        glfwSetScrollCallback(data_->window, ScrollCallback);
    }

    void Input::set_scroll(float offset)
    {
        LEEP_ASSERT(data_, "A window is needed");
        data_->current.scroll = offset;
    }

    float Input::scroll() const
    {
        LEEP_ASSERT(data_, "A window is needed");
        return data_->previous.scroll;
    }

    bool Input::isButtonPressed(Button b) const
    {
        LEEP_ASSERT(data_, "A window is needed");
        if (GM.ui_tools().wantKeyboard())
        {
            return false;
        }

        switch (b)
        {
            case Button::UP: return data_->current.up;
            case Button::DOWN: return data_->current.down;
            case Button::LEFT: return data_->current.left;
            case Button::RIGHT: return data_->current.right;
            case Button::B1: return data_->current.button_1;
            case Button::B2: return data_->current.button_2;
            case Button::B3: return data_->current.button_3;
            case Button::B4: return data_->current.button_4;
            case Button::MOUSE_LEFT: return data_->current.mouse_left;
            case Button::MOUSE_RIGHT: return data_->current.mouse_right;
            default: LEEP_CORE_ERROR("isButtonPressed swich default.\n"); return false;
        }
    }

    bool Input::isButtonDown(Button b) const
    {
        LEEP_ASSERT(data_, "A window is needed");
        if (GM.ui_tools().wantKeyboard())
        {
            return false;
        }

        if (!isButtonPressed(b))
            return false;

        switch(b)
        {
            case Button::UP: return !data_->previous.up;
            case Button::DOWN: return !data_->previous.down;
            case Button::LEFT: return !data_->previous.left;
            case Button::RIGHT: return !data_->previous.right;
            case Button::B1: return !data_->previous.button_1;
            case Button::B2: return !data_->previous.button_2;
            case Button::B3: return !data_->previous.button_3;
            case Button::B4: return !data_->previous.button_4;
            case Button::MOUSE_LEFT: return !data_->previous.mouse_left;
            case Button::MOUSE_RIGHT: return !data_->previous.mouse_right;
            default: LEEP_CORE_ERROR("isButtonDown swich default.\n"); return false;
        }
    }

    bool Input::isButtonUp(Button b) const
    {
        LEEP_ASSERT(data_, "A window is needed");
        if (GM.ui_tools().wantKeyboard())
        {
            return false;
        }

        if (isButtonPressed(b))
            return false;

        switch(b)
        {
            case Button::UP: return data_->previous.up;
            case Button::DOWN: return data_->previous.down;
            case Button::LEFT: return data_->previous.left;
            case Button::RIGHT: return data_->previous.right;
            case Button::B1: return data_->previous.button_1;
            case Button::B2: return data_->previous.button_2;
            case Button::B3: return data_->previous.button_3;
            case Button::B4: return data_->previous.button_4;
            case Button::MOUSE_LEFT: return data_->previous.mouse_left;
            case Button::MOUSE_RIGHT: return data_->previous.mouse_right;
            default: LEEP_CORE_ERROR("isButtonUp swich default.\n"); return false;
        }
    }

    float Input::mouseX() const
    {
        LEEP_ASSERT(data_, "A window is needed");
        return data_->current.mouse_position_x;
    }

    float Input::mouseY() const
    {
        LEEP_ASSERT(data_, "A window is needed");
        return data_->current.mouse_position_y;
    }

    void Input::updateInput()
    {
        LEEP_ASSERT(data_, "A window is needed");
        double x, y;
        data_->previous = data_->current;
        data_->current.up = glfwGetKey(data_->window, ToGLFW(Button::UP));
        data_->current.down = glfwGetKey(data_->window, ToGLFW(Button::DOWN));
        data_->current.left = glfwGetKey(data_->window, ToGLFW(Button::LEFT));
        data_->current.right = glfwGetKey(data_->window, ToGLFW(Button::RIGHT));
        data_->current.button_1 = glfwGetKey(data_->window, ToGLFW(Button::B1));
        data_->current.button_2 = glfwGetKey(data_->window, ToGLFW(Button::B2));
        data_->current.button_3 = glfwGetKey(data_->window, ToGLFW(Button::B3));
        data_->current.button_4 = glfwGetKey(data_->window, ToGLFW(Button::B4));
        data_->current.mouse_left = glfwGetMouseButton(data_->window, ToGLFW(Button::MOUSE_LEFT));
        data_->current.mouse_right = glfwGetMouseButton(data_->window, ToGLFW(Button::MOUSE_RIGHT));
        glfwGetCursorPos(data_->window, &x, &y);
        data_->current.mouse_position_x = (float)x;
        data_->current.mouse_position_y = (float)y;
        data_->current.scroll = 0.0f;
    }

    void Input::disableCursor() const
    {
        LEEP_ASSERT(data_, "A window is needed");
        glfwSetInputMode(data_->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Input::enableCursor() const
    {
        LEEP_ASSERT(data_, "A window is needed");
        glfwSetInputMode(data_->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
