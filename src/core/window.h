#ifndef __MOTORET_WINDOW_H__
#define __MOTORET_WINDOW_H__ 1

namespace motoret {
	class Window {
	public:
		Window();
		~Window();
		bool createWindow(uint16_t w, uint16_t h, bool limit_framerate = true);
		bool windowShouldClose();
		int32 width() const;
		int32 height() const;
		void swap();
		void pollEvents();

	private:
		struct WindowData;
		WindowData *data_;
	};
}

#endif // __MOTORET_WINDOW_H__