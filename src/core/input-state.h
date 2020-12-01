// Lucas Merenciano Martinez <lucasmermar@gmail.com>
#ifndef __LEEP_CORE_INPUT_STATE_H__
#define __LEEP_CORE_INPUT_STATE_H__ 1

namespace leep
{
    struct InputState
    {
        bool up;
        bool left;
        bool right;
        bool down;
        bool button_1;
        bool button_2;
        bool button_3;
        bool button_4;
        bool mouse_left;
        bool mouse_right;
        float mouse_position_x;
        float mouse_position_y;
        float scroll;
    };
}

#endif // __LEEP_CORE_INPUT_STATE_H__