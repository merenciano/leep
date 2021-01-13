#include "window.h"
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
		float time;
		float delta_time;
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
		delete data_;
#endif
	}

	bool Window::createWindow(uint16_t w, uint16_t h, bool limit_framerate)
	{
		if (!glfwInit())
		{
			return false;
		}
		
		if (data_)
		{
			return false; // window already created
		}

		data_ = new WindowData();
		data_->time = (float)glfwGetTime();
		data_->delta_time = 1.0f / 60.0f;
		data_->window = glfwCreateWindow(w, h, "LEEP", NULL, NULL);
		if (!data_->window)
		{
			glfwTerminate();
			return false;
		}

		// I call this function here because motoret only supports one window
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
		LEEP_ASSERT(data_, "The window has not been created yet");
		return glfwWindowShouldClose(data_->window);
	}

	void Window::swap()
	{
		LEEP_ASSERT(data_, "The window has not been created yet");
		glfwSwapBuffers(data_->window);
		float time = (float)glfwGetTime();
		data_->delta_time = time - data_->time;
		data_->time = time;
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	int32_t Window::width() const
	{
		LEEP_ASSERT(data_, "The window has not been created yet");
		int32_t width;
		glfwGetWindowSize(data_->window, &width, NULL);
		return width;
	}

	int32_t Window::height() const
	{
		LEEP_ASSERT(data_, "The window has not been created yet");
		int32_t height;
		glfwGetWindowSize(data_->window, nullptr, &height);
		return height;
	}

	float Window::time() const
	{
		LEEP_ASSERT(data_, "The window has not been created yet");
		return data_->time;
	}

	float Window::delta_time() const
	{
		LEEP_ASSERT(data_, "The window has not been created yet");
		return data_->delta_time;
	}
}