#include "io.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "tools/imgui-tools.h"

static GLFWwindow *internal_window = NULL;
static THE_InputState curr;
static THE_InputState prev;
static bool capture_kb;
static bool capture_mouse;

static s32 Internal_ToGLFW_Map[THE_INPUT_COUNT] = {
	GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_S,
	GLFW_KEY_SPACE, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_LEFT_SHIFT,
	GLFW_MOUSE_BUTTON_1, GLFW_MOUSE_BUTTON_2
};

static void ScrollCallback(GLFWwindow *window, double x_offset, double y_offset)
{
	THE_InputSetScroll((float)y_offset);
}

bool THE_IOInit(const char *title, s32 width, s32 height, bool limit_framerate)
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

	glfwSetScrollCallback(internal_window, ScrollCallback);
	THE_UIToolsInit(internal_window);

	curr.input_bitmap = 0;
	prev.input_bitmap = 0;
	curr.scroll = 0.0f;
	prev.scroll = 0.0f;
	curr.mouse_x = 0.0f;
	curr.mouse_y = 0.0f;
	prev.mouse_x = 0.0f;
	prev.mouse_y = 0.0f;
	capture_kb = true;
	capture_mouse = true;

	return true;
}

void THE_IOPollEvents()
{
	glfwPollEvents();
}

// OUTPUT  ------------------------------

s32 THE_WindowShouldClose()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	return glfwWindowShouldClose(internal_window);
}

void THE_WindowSwapBuffers()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	glfwSwapBuffers(internal_window);
}



s32 THE_WindowGetWidth()
{
	THE_ASSERT(internal_window, "The IO system is uninitalizedt");
	s32 width;
	glfwGetWindowSize(internal_window, &width, NULL);
	return width;
}

s32 THE_WindowGetHeight()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	s32 height;
	glfwGetWindowSize(internal_window, NULL, &height);
	return height;
}

// INPUT ------------------------------

void THE_InputSetScroll(float offset)
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	if (capture_mouse) {
		curr.scroll = offset;
	}
}

float THE_InputGetScroll()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	return prev.scroll;
}

bool THE_InputIsButtonPressed(THE_Input button)
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	THE_ASSERT(button != THE_INPUT_COUNT, "Invalid button");
	if (!capture_kb) {
		return false;
	}

	return curr.input_bitmap & (1 << button);
}

bool THE_InputIsButtonDown(THE_Input button)
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	THE_ASSERT(button != THE_INPUT_COUNT, "Invalid button");
	if (!THE_InputIsButtonPressed(button)) {
		return false;
	}

	return !(prev.input_bitmap & (1 << button));
}

bool THE_InputIsButtonUp(THE_Input button)
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	THE_ASSERT(button != THE_INPUT_COUNT, "Invalid button");

	if (!capture_kb) {
		return false;
	}

	if (THE_InputIsButtonPressed(button)) {
		return false;
	}

	return prev.input_bitmap & (1 << button);
}

float THE_InputGetMouseX()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	return curr.mouse_x;
}

float THE_InputGetMouseY()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	return curr.mouse_y;
}

void THE_InputUpdate()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	prev = curr;
	curr.scroll = 0.0f;
	curr.input_bitmap = 0;
	for (s32 i = 0; i < THE_MOUSE_LEFT; ++i) {
		curr.input_bitmap |= ((bool)glfwGetKey(internal_window, Internal_ToGLFW_Map[i]) << i);
	}
	curr.input_bitmap |= ((bool)glfwGetMouseButton(internal_window, Internal_ToGLFW_Map[THE_MOUSE_LEFT]) << THE_MOUSE_LEFT);
	curr.input_bitmap |= ((bool)glfwGetMouseButton(internal_window, Internal_ToGLFW_Map[THE_MOUSE_RIGHT]) << THE_MOUSE_RIGHT);

	double x, y;
	glfwGetCursorPos(internal_window, &x, &y);
	curr.mouse_x = (float)x;
	curr.mouse_y = (float)y;
}

void THE_InputDisableCursor()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	glfwSetInputMode(internal_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void THE_InputEnableCursor()
{
	THE_ASSERT(internal_window, "The IO system is uninitalized");
	glfwSetInputMode(internal_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void THE_InputCapture(bool mouse, bool kb)
{
	capture_kb = kb;
	capture_mouse = mouse;
}
