#ifndef THE_CORE_IO_H
#define THE_CORE_IO_H

#include "Cdefinitions.h"

typedef enum {
	THE_KEY_UP = 0,
	THE_KEY_LEFT,
	THE_KEY_RIGHT,
	THE_KEY_DOWN,
	THE_KEY_1,
	THE_KEY_2,
	THE_KEY_3,
	THE_KEY_4,
	THE_MOUSE_LEFT,
	THE_MOUSE_RIGHT,
	THE_INPUT_COUNT
} THE_Input;

typedef struct {
	float mouse_x;
	float mouse_y;
	float scroll;
	u32 input_bitmap;
} THE_InputState;

bool THE_IOInit(const char *title, s32 width, s32 height, bool limit_framerate);
void THE_IOPollEvents();

s32 THE_WindowShouldClose();
void THE_WindowSwapBuffers();
s32 THE_WindowGetWidth();
s32 THE_WindowGetHeight();

void THE_InputUpdate();
void THE_InputSetScroll(float offset);
float THE_InputGetScroll();
bool THE_InputIsButtonPressed(THE_Input button);
bool THE_InputIsButtonDown(THE_Input button);
bool THE_InputIsButtonUp(THE_Input button);
float THE_InputGetMouseX();
float THE_InputGetMouseY();
void THE_InputDisableCursor();
void THE_InputEnableCursor();
void THE_InputCapture(bool mouse, bool kb);

#endif
