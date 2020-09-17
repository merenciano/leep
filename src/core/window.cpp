#include "window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace motoret;

struct motoret::Window::WindowData {
	GLFWwindow *window;
};

Window::Window()
{
	data_ = nullptr;
}

Window::~Window()
{
	glfwTerminate();
	delete data_;
}

bool Window::createWindow(uint16_t w, uint16_t h, bool limit_framerate)
{
	if (!glfwInit())
	{
		return false;
	}
	
	if (data_)
	{
		return false; // Window already created
	}

	data_ = new WindowData();
	data_->window = glfwCreateWindow(w, h, "MOTORET", NULL, NULL);
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

	return true;
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(data_->window);
}

void Window::swap()
{
	glfwSwapBuffers(data_->window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

int32_t Window::width() const
{
	int32_t width;
	glfwGetWindowSize(data_->window, &width, nullptr);
	return width;
}

int32_t Window::height() const
{
	int32_t height;
	glfwGetWindowSize(data_->window, nullptr, &height);
	return height;
}