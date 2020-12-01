#include "input.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "GLFW/glfw3.h"

namespace leep
{
    struct Input::WindowData
    {
        GLFWwindow *window;
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
        GM.input().set_scroll((float)y_offset);
    }

    Input::Input()
    {

    }

    Input::~Input()
    {
        delete data_;
    }

    void Input::init(void *window)
    {
        if (window == nullptr)
            return;
        
        if (data_)
            return;

        data_ = new WindowData();
        data_->window = (GLFWwindow*)window;
        glfwSetScrollCallback(data_->window, ScrollCallback);
    }

    void Input::set_scroll(float offset)
    {
        current_.scroll = offset;
    }

    float Input::scroll() const
    {
        return previous_.scroll;
    }

    bool Input::isButtonPressed(Button b) const
    {
        switch (b)
        {
            case Button::UP: return current_.up;
            case Button::DOWN: return current_.down;
            case Button::LEFT: return current_.left;
            case Button::RIGHT: return current_.right;
            case Button::B1: return current_.button_1;
            case Button::B2: return current_.button_2;
            case Button::B3: return current_.button_3;
            case Button::B4: return current_.button_4;
            case Button::MOUSE_LEFT: return current_.mouse_left;
            case Button::MOUSE_RIGHT: return current_.mouse_right;
            default: LEEP_CORE_ERROR("isButtonPressed swich default.\n"); return false;
        }
    }

    bool Input::isButtonDown(Button b) const
    {
        if (!isButtonPressed(b))
            return false;

        switch(b)
        {
            case Button::UP: return !previous_.up;
            case Button::DOWN: return !previous_.down;
            case Button::LEFT: return !previous_.left;
            case Button::RIGHT: return !previous_.right;
            case Button::B1: return !previous_.button_1;
            case Button::B2: return !previous_.button_2;
            case Button::B3: return !previous_.button_3;
            case Button::B4: return !previous_.button_4;
            case Button::MOUSE_LEFT: return !previous_.mouse_left;
            case Button::MOUSE_RIGHT: return !previous_.mouse_right;
            default: LEEP_CORE_ERROR("isButtonDown swich default.\n"); return false;
        }
    }

    bool Input::isButtonUp(Button b) const
    {
        if (isButtonPressed(b))
            return false;

        switch(b)
        {
            case Button::UP: return previous_.up;
            case Button::DOWN: return previous_.down;
            case Button::LEFT: return previous_.left;
            case Button::RIGHT: return previous_.right;
            case Button::B1: return previous_.button_1;
            case Button::B2: return previous_.button_2;
            case Button::B3: return previous_.button_3;
            case Button::B4: return previous_.button_4;
            case Button::MOUSE_LEFT: return previous_.mouse_left;
            case Button::MOUSE_RIGHT: return previous_.mouse_right;
            default: LEEP_CORE_ERROR("isButtonUp swich default.\n"); return false;
        }
    }

    float Input::mouseX() const
    {
        return current_.mouse_position_x;
    }

    float Input::mouseY() const
    {
        return current_.mouse_position_y;
    }

    void Input::updateInput()
    {
        double x, y;
        previous_ = current_;
        current_.up = glfwGetKey(data_->window, ToGLFW(Button::UP));
        current_.down = glfwGetKey(data_->window, ToGLFW(Button::DOWN));
        current_.left = glfwGetKey(data_->window, ToGLFW(Button::LEFT));
        current_.right = glfwGetKey(data_->window, ToGLFW(Button::RIGHT));
        current_.button_1 = glfwGetKey(data_->window, ToGLFW(Button::B1));
        current_.button_2 = glfwGetKey(data_->window, ToGLFW(Button::B2));
        current_.button_3 = glfwGetKey(data_->window, ToGLFW(Button::B3));
        current_.button_4 = glfwGetKey(data_->window, ToGLFW(Button::B4));
        current_.mouse_left = glfwGetMouseButton(data_->window, ToGLFW(Button::MOUSE_LEFT));
        current_.mouse_right = glfwGetMouseButton(data_->window, ToGLFW(Button::MOUSE_RIGHT));
        glfwGetCursorPos(data_->window, &x, &y);
        current_.mouse_position_x = (float)x;
        current_.mouse_position_y = (float)y;
        current_.scroll = 0.0f;
    }

    void Input::disableCursor() const
    {
        glfwSetInputMode(data_->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Input::enableCursor() const
    {
        glfwSetInputMode(data_->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
