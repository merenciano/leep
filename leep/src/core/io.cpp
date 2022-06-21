#include "io.h"

#include "Cdefinitions.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

static GLFWwindow *internal_window = NULL;

bool THE_WindowInit(const char *title, s32 width, s32 height, bool limit_framerate)
{
	if (!glfwInit() || internal_window) { // glfwInit fail or window init already
		return false;
	}

	internal_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!internal_window) {
		glfwTerminate();
		return false;
	}

	// I call this function here because THE only supports one window
	glfwMakeContextCurrent(internal_window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	if (!limit_framerate) {
		glfwSwapInterval(0);
	}

	//GM.input().init(data_->window);
	//GM.ui_tools().init(data_->window);

	return true;
}

s32 THE_WindowShouldClose()
{
	THE_ASSERT(internal_window && "The window has not been created yet");
	return glfwWindowShouldClose(internal_window);
}

void THE_WindowSwapBuffers()
{
	THE_ASSERT(internal_window && "The window has not been created yet");
	glfwSwapBuffers(internal_window);
}

void THE_PollIOEvents()
{
	glfwPollEvents();
}

s32 THE_WindowGetWidth()
{
	THE_ASSERT(internal_window && "The window has not been created yet");
	s32 width;
	glfwGetWindowSize(internal_window, &width, NULL);
	return width;
}

s32 THE_WindowGetHeight()
{
	THE_ASSERT(internal_window && "The window has not been created yet");
	s32 height;
	glfwGetWindowSize(internal_window, NULL, &height);
	return height;
}

// TODO VOY POR AQUI
// Implementar Input aqui tambien

