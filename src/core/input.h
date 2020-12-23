#ifndef __LEEP_CORE_INPUT_H__
#define __LEEP_CORE_INPUT_H__ 1

#include "core/input-state.h"

namespace leep
{
    enum class Button
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        B1,
        B2,
        B3,
        B4,
        MOUSE_LEFT,
        MOUSE_RIGHT
    };

    class Input
    {
    public:
        Input();
        ~Input();

        void init(void *window);
        void updateInput();
        void set_scroll(float offset);
        void enableCursor() const;
        void disableCursor() const;

        bool isButtonDown(Button b) const;
        bool isButtonPressed(Button b) const;
        bool isButtonUp(Button b) const;
        
        float mouseX() const;
        float mouseY() const;
        float scroll() const;

    private:
        struct InputData;
        InputData *data_;
    };
}

#endif // __LEEP_CORE_INPUT_H__