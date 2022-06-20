#include "window.h"
#include "core/Cdefinitions.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "tools/imgui-tools.h"
#include "core/input.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace leep
{
	struct leep::Window::WindowData {
		GLFWwindow *window;
	};

	Window::Window()
	{
		data_ = nullptr;
	}

	Window::~Window()
	{
		// The window class lifetime will be the same as the entire program
		// so all of this will be freed anyway when the program closes.
#ifdef LEEP_DEBUG
		//glfwTerminate(); //crashes
		GM.memory().generalFree(data_);
#endif
	}

	bool Window::createWindow(const char *title, uint16_t w, uint16_t h, bool limit_framerate)
	{
		if (!glfwInit())
		{
			return false;
		}
		
		if (data_)
		{
			return false; // window already created
		}

		data_ = GM.memory().generalAllocT<WindowData>(1);
		data_->window = glfwCreateWindow(w, h, title, NULL, NULL);
		if (!data_->window)
		{
			glfwTerminate();
			return false;
		}

		// I call this function here because leep only supports one window
		glfwMakeContextCurrent(data_->window);

		gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

		if (!limit_framerate)
			glfwSwapInterval(0);

		GM.input().init(data_->window);
		GM.ui_tools().init(data_->window);

		return true;
	}

	bool Window::windowShouldClose()
	{
		THE_ASSERT(data_ && "The window has not been created yet");
		return glfwWindowShouldClose(data_->window);
	}

	void Window::swap()
	{
		THE_ASSERT(data_ && "The window has not been created yet");
		glfwSwapBuffers(data_->window);
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	int32_t Window::width() const
	{
		THE_ASSERT(data_ && "The window has not been created yet");
		int32_t width;
		glfwGetWindowSize(data_->window, &width, NULL);
		return width;
	}

	int32_t Window::height() const
	{
		THE_ASSERT(data_ && "The window has not been created yet");
		int32_t height;
		glfwGetWindowSize(data_->window, nullptr, &height);
		return height;
	}

	float Window::fwidth() const
	{
		THE_ASSERT(data_ && "The window has not been created yet");
		int width;
		glfwGetWindowSize(data_->window, &width, NULL);
		return (float)width;
	}

	float Window::fheight() const
	{
		THE_ASSERT(data_ && "The window has not been created yet");
		int height;
		glfwGetWindowSize(data_->window, nullptr, &height);
		return (float)height;
	}
}