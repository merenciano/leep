#ifndef __MOTORET_RENDER_CLEAR_H__
#define __MOTORET_RENDER_CLEAR_H__ 1

#include "display-list-command.h"

namespace motoret
{
    class Clear : public DisplayListCommand
    {
    public:
        Clear();
        ~Clear();

        virtual void executeCommand() const override;

        Clear& set_clear_color(float r, float g, float b, float a);
        Clear& set_clear_buffer(bool color, bool depth, bool stencil);

    private:
        float clear_color_[4];
        bool color_;
        bool depth_;
        bool stencil_;
    };
}

#endif // __MOTORET_RENDER_CLEAR_H__