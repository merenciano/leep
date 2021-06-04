#ifndef __LEEP_CORE_WINDOW_H__
#define __LEEP_CORE_WINDOW_H__ 1

#include <stdint.h>

namespace leep
{
	class Window {
	public:
		Window();
		~Window();
		bool createWindow(uint16_t w, uint16_t h, bool limit_framerate = true);
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

#endif // __LEEP_CORE_WINDOW_H__