#ifndef __THE_CORE_WINDOW_H__
#define __THE_CORE_WINDOW_H__

#include <stdint.h>

typedef struct {
	struct WindowData;
	WindowData *data;
} THE_Window;

namespace leep
{
    class Window {
    public:
        Window();
        ~Window();
        bool createWindow(const char *title, uint16_t w, uint16_t h, bool limit_framerate = true);
        bool windowShouldClose();
        int32_t width() const;
        int32_t height() const;
        float fwidth() const;
        float fheight() const;
        void swap();
        void pollEvents();
    
    private:
        struct WindowData;
        WindowData *data_;
    };
}

#endif
