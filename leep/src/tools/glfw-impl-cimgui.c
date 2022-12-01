#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_GLFW

#include "cimgui.h"
#include "cimgui_impl.h"
#include "GLFW/glfw3.h"

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <assert.h>

typedef struct ImGui_ImplGlfw_Data ImGui_ImplGlfw_Data;

#define GLFW_VERSION_COMBINED           (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 + GLFW_VERSION_REVISION)
#define GLFW_HAS_WINDOW_TOPMOST         (GLFW_VERSION_COMBINED >= 3200) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED         (GLFW_VERSION_COMBINED >= 3300) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA           (GLFW_VERSION_COMBINED >= 3300) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI        (GLFW_VERSION_COMBINED >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN                 (GLFW_VERSION_COMBINED >= 3200) // 3.2+ glfwCreateWindowSurface
#define GLFW_HAS_FOCUS_WINDOW           (GLFW_VERSION_COMBINED >= 3200) // 3.2+ glfwFocusWindow
#define GLFW_HAS_FOCUS_ON_SHOW          (GLFW_VERSION_COMBINED >= 3300) // 3.3+ GLFW_FOCUS_ON_SHOW
#define GLFW_HAS_MONITOR_WORK_AREA      (GLFW_VERSION_COMBINED >= 3300) // 3.3+ glfwGetMonitorWorkarea
#define GLFW_HAS_OSX_WINDOW_POS_FIX     (GLFW_VERSION_COMBINED >= 3301) // 3.3.1+ Fixed: Resizing window repositions it on MacOS #1553
#ifdef GLFW_RESIZE_NESW_CURSOR          // Let's be nice to people who pulled GLFW between 2019-04-16 (3.4 define) and 2019-11-29 (cursors defines) // FIXME: Remove when GLFW 3.4 is released?
#define GLFW_HAS_NEW_CURSORS            (GLFW_VERSION_COMBINED >= 3400) // 3.4+ GLFW_RESIZE_ALL_CURSOR, GLFW_RESIZE_NESW_CURSOR, GLFW_RESIZE_NWSE_CURSOR, GLFW_NOT_ALLOWED_CURSOR
#else
#define GLFW_HAS_NEW_CURSORS            (0)
#endif
#ifdef GLFW_MOUSE_PASSTHROUGH           // Let's be nice to people who pulled GLFW between 2019-04-16 (3.4 define) and 2020-07-17 (passthrough)
#define GLFW_HAS_MOUSE_PASSTHROUGH      (GLFW_VERSION_COMBINED >= 3400) // 3.4+ GLFW_MOUSE_PASSTHROUGH
#else
#define GLFW_HAS_MOUSE_PASSTHROUGH      (0)
#endif
#define GLFW_HAS_GAMEPAD_API            (GLFW_VERSION_COMBINED >= 3300) // 3.3+ glfwGetGamepadState() new api
#define GLFW_HAS_GETKEYNAME             (GLFW_VERSION_COMBINED >= 3200) // 3.2+ glfwGetKeyName()

// GLFW data
typedef enum GlfwClientApi
{
    GlfwClientApi_Unknown,
    GlfwClientApi_OpenGL,
    GlfwClientApi_Vulkan
} GlfwClientApi;

struct ImGui_ImplGlfw_Data
{
	GLFWwindow *window;
	GlfwClientApi client_api;
	double time;
	GLFWwindow *mouse_window;
	GLFWcursor *mouse_cursors[ImGuiMouseCursor_COUNT];
	ImVec2 last_valid_mouse_pos;
	GLFWwindow *key_owner_windows[GLFW_KEY_LAST];
	bool installed_callbacks;
	bool want_update_monitors;

	GLFWwindowfocusfun prev_window_focus;
	GLFWcursorposfun prev_cursor_pos;
	GLFWcursorenterfun prev_cursor_enter;
	GLFWmousebuttonfun prev_mousebutton;
	GLFWscrollfun prev_scroll;
	GLFWkeyfun prev_key;
	GLFWcharfun prev_char;
	GLFWmonitorfun prev_monitor;
};

static ImVec2 CImVec2(float x, float y)
{
    ImVec2 v = {x, y};
    return v;
}

static ImGui_ImplGlfw_Data* ImGui_ImplGlfw_GetBackendData()
{
    return igGetCurrentContext() ? (ImGui_ImplGlfw_Data*)igGetIO()->BackendPlatformUserData : NULL;
}

// Forward Declarations
static void ImGui_ImplGlfw_UpdateMonitors();
static void ImGui_ImplGlfw_InitPlatformInterface();
static void ImGui_ImplGlfw_ShutdownPlatformInterface();

// Functions
static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data)
{
    return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text)
{
    glfwSetClipboardString((GLFWwindow*)user_data, text);
}

static ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int key)
{
    switch (key)
    {
        case GLFW_KEY_TAB: return ImGuiKey_Tab;
        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
        case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case GLFW_KEY_HOME: return ImGuiKey_Home;
        case GLFW_KEY_END: return ImGuiKey_End;
        case GLFW_KEY_INSERT: return ImGuiKey_Insert;
        case GLFW_KEY_DELETE: return ImGuiKey_Delete;
        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case GLFW_KEY_SPACE: return ImGuiKey_Space;
        case GLFW_KEY_ENTER: return ImGuiKey_Enter;
        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
        case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case GLFW_KEY_COMMA: return ImGuiKey_Comma;
        case GLFW_KEY_MINUS: return ImGuiKey_Minus;
        case GLFW_KEY_PERIOD: return ImGuiKey_Period;
        case GLFW_KEY_SLASH: return ImGuiKey_Slash;
        case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
        case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
        case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
        case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
        case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
        case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
        case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
        case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
        case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
        case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
        case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
        case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
        case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
        case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
        case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case GLFW_KEY_MENU: return ImGuiKey_Menu;
        case GLFW_KEY_0: return ImGuiKey_0;
        case GLFW_KEY_1: return ImGuiKey_1;
        case GLFW_KEY_2: return ImGuiKey_2;
        case GLFW_KEY_3: return ImGuiKey_3;
        case GLFW_KEY_4: return ImGuiKey_4;
        case GLFW_KEY_5: return ImGuiKey_5;
        case GLFW_KEY_6: return ImGuiKey_6;
        case GLFW_KEY_7: return ImGuiKey_7;
        case GLFW_KEY_8: return ImGuiKey_8;
        case GLFW_KEY_9: return ImGuiKey_9;
        case GLFW_KEY_A: return ImGuiKey_A;
        case GLFW_KEY_B: return ImGuiKey_B;
        case GLFW_KEY_C: return ImGuiKey_C;
        case GLFW_KEY_D: return ImGuiKey_D;
        case GLFW_KEY_E: return ImGuiKey_E;
        case GLFW_KEY_F: return ImGuiKey_F;
        case GLFW_KEY_G: return ImGuiKey_G;
        case GLFW_KEY_H: return ImGuiKey_H;
        case GLFW_KEY_I: return ImGuiKey_I;
        case GLFW_KEY_J: return ImGuiKey_J;
        case GLFW_KEY_K: return ImGuiKey_K;
        case GLFW_KEY_L: return ImGuiKey_L;
        case GLFW_KEY_M: return ImGuiKey_M;
        case GLFW_KEY_N: return ImGuiKey_N;
        case GLFW_KEY_O: return ImGuiKey_O;
        case GLFW_KEY_P: return ImGuiKey_P;
        case GLFW_KEY_Q: return ImGuiKey_Q;
        case GLFW_KEY_R: return ImGuiKey_R;
        case GLFW_KEY_S: return ImGuiKey_S;
        case GLFW_KEY_T: return ImGuiKey_T;
        case GLFW_KEY_U: return ImGuiKey_U;
        case GLFW_KEY_V: return ImGuiKey_V;
        case GLFW_KEY_W: return ImGuiKey_W;
        case GLFW_KEY_X: return ImGuiKey_X;
        case GLFW_KEY_Y: return ImGuiKey_Y;
        case GLFW_KEY_Z: return ImGuiKey_Z;
        case GLFW_KEY_F1: return ImGuiKey_F1;
        case GLFW_KEY_F2: return ImGuiKey_F2;
        case GLFW_KEY_F3: return ImGuiKey_F3;
        case GLFW_KEY_F4: return ImGuiKey_F4;
        case GLFW_KEY_F5: return ImGuiKey_F5;
        case GLFW_KEY_F6: return ImGuiKey_F6;
        case GLFW_KEY_F7: return ImGuiKey_F7;
        case GLFW_KEY_F8: return ImGuiKey_F8;
        case GLFW_KEY_F9: return ImGuiKey_F9;
        case GLFW_KEY_F10: return ImGuiKey_F10;
        case GLFW_KEY_F11: return ImGuiKey_F11;
        case GLFW_KEY_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
    }
}

static int ImGui_ImplGlfw_KeyToModifier(int key)
{
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
        return GLFW_MOD_CONTROL;
    if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
        return GLFW_MOD_SHIFT;
    if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
        return GLFW_MOD_ALT;
    if (key == GLFW_KEY_LEFT_SUPER || key == GLFW_KEY_RIGHT_SUPER)
        return GLFW_MOD_SUPER;
    return 0;
}

static void ImGui_ImplGlfw_UpdateKeyModifiers(int mods)
{
    ImGuiIO *io = igGetIO();
    ImGuiIO_AddKeyEvent(io, ImGuiMod_Ctrl, (mods & GLFW_MOD_CONTROL) != 0);
    ImGuiIO_AddKeyEvent(io, ImGuiMod_Shift, (mods & GLFW_MOD_SHIFT) != 0);
    ImGuiIO_AddKeyEvent(io, ImGuiMod_Alt, (mods & GLFW_MOD_ALT) != 0);
    ImGuiIO_AddKeyEvent(io, ImGuiMod_Super, (mods & GLFW_MOD_SUPER) != 0);
}

void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (bd->prev_mousebutton != NULL && window == bd->window)
        bd->prev_mousebutton(window, button, action, mods);

    ImGui_ImplGlfw_UpdateKeyModifiers(mods);

    if (button >= 0 && button < ImGuiMouseButton_COUNT)
		ImGuiIO_AddMouseButtonEvent(io, button, action == GLFW_PRESS);
}

void ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (bd->prev_scroll != NULL && window == bd->window)
        bd->prev_scroll(window, xoffset, yoffset);

    ImGuiIO_AddMouseWheelEvent(io, (float)xoffset, (float)yoffset);
}

static int ImGui_ImplGlfw_TranslateUntranslatedKey(int key, int scancode)
{
#if GLFW_HAS_GETKEYNAME && !defined(__EMSCRIPTEN__)
    // GLFW 3.1+ attempts to "untranslate" keys, which goes the opposite of what every other framework does, making using lettered shortcuts difficult.
    // (It had reasons to do so: namely GLFW is/was more likely to be used for WASD-type game controls rather than lettered shortcuts, but IHMO the 3.1 change could have been done differently)
    // See https://github.com/glfw/glfw/issues/1502 for details.
    // Adding a workaround to undo this (so our keys are translated->untranslated->translated, likely a lossy process).
    // This won't cover edge cases but this is at least going to cover common cases.
    if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL)
        return key;
    GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
    const char* key_name = glfwGetKeyName(key, scancode);
    glfwSetErrorCallback(prev_error_callback);
#if (GLFW_VERSION_COMBINED >= 3300) // Eat errors (see #5908)
    (void)glfwGetError(NULL);
#endif
    if (key_name && key_name[0] != 0 && key_name[1] == 0)
    {
        const char *p;
        const char char_names[] = "`-=[]\\,;\'./";
        const int char_keys[] = { GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS, GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_COMMA, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD, GLFW_KEY_SLASH, 0 };
        IM_ASSERT(IM_ARRAYSIZE(char_names) == IM_ARRAYSIZE(char_keys));
        if (key_name[0] >= '0' && key_name[0] <= '9')               { key = GLFW_KEY_0 + (key_name[0] - '0'); }
        else if (key_name[0] >= 'A' && key_name[0] <= 'Z')          { key = GLFW_KEY_A + (key_name[0] - 'A'); }
        else if (key_name[0] >= 'a' && key_name[0] <= 'z')          { key = GLFW_KEY_A + (key_name[0] - 'a'); }
        else if ((p = strchr(char_names, key_name[0])))             { key = char_keys[p - char_names]; }
    }
    // if (action == GLFW_PRESS) printf("key %d scancode %d name '%s'\n", key, scancode, key_name);
#else
    IM_UNUSED(scancode);
#endif
    return key;
}

void ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (bd->prev_key != NULL && window == bd->window)
        bd->prev_key(window, keycode, scancode, action, mods);

    if (action != GLFW_PRESS && action != GLFW_RELEASE)
        return;

    // Workaround: X11 does not include current pressed/released modifier key in 'mods' flags. https://github.com/glfw/glfw/issues/1630
    int keycode_to_mod;
    if ((keycode_to_mod = ImGui_ImplGlfw_KeyToModifier(keycode)))
        mods = (action == GLFW_PRESS) ? (mods | keycode_to_mod) : (mods & ~keycode_to_mod);
    ImGui_ImplGlfw_UpdateKeyModifiers(mods);

    if (keycode >= 0 && keycode < sizeof(bd->key_owner_windows))
        bd->key_owner_windows[keycode] = (action == GLFW_PRESS) ? window : NULL;

    keycode = ImGui_ImplGlfw_TranslateUntranslatedKey(keycode, scancode);

    ImGuiKey imgui_key = ImGui_ImplGlfw_KeyToImGuiKey(keycode);
    ImGuiIO_AddKeyEvent(io, imgui_key, (action == GLFW_PRESS));
    ImGuiIO_SetKeyEventNativeData(io, imgui_key, keycode, scancode, -1); // To support legacy indexing (<1.87 user code)
}

void ImGui_ImplGlfw_WindowFocusCallback(GLFWwindow* window, int focused)
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (bd->prev_window_focus != NULL && window == bd->window)
        bd->prev_window_focus(window, focused);

    ImGuiIO_AddFocusEvent(io, focused != 0);
}

void ImGui_ImplGlfw_CursorPosCallback(GLFWwindow* window, double x, double y)
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (bd->prev_cursor_pos != NULL && window == bd->window)
        bd->prev_cursor_pos(window, x, y);
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        return;

    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        int window_x, window_y;
        glfwGetWindowPos(window, &window_x, &window_y);
        x += window_x;
        y += window_y;
    }
    ImGuiIO_AddMousePosEvent(io, (float)x, (float)y);
    bd->last_valid_mouse_pos = CImVec2((float)x, (float)y);
}

// Workaround: X11 seems to send spurious Leave/Enter events which would make us lose our position,
// so we back it up and restore on Leave/Enter (see https://github.com/ocornut/imgui/issues/4984)
void ImGui_ImplGlfw_CursorEnterCallback(GLFWwindow* window, int entered)
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (bd->prev_cursor_enter != NULL && window == bd->window)
        bd->prev_cursor_enter(window, entered);
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        return;

    if (entered)
    {
        bd->mouse_window = window;
        ImGuiIO_AddMousePosEvent(io, bd->last_valid_mouse_pos.x, bd->last_valid_mouse_pos.y);
    }
    else if (!entered && bd->mouse_window == window)
    {
        bd->last_valid_mouse_pos = io->MousePos;
        bd->mouse_window = NULL;
        ImGuiIO_AddMousePosEvent(io, -FLT_MAX, -FLT_MAX);
    }
}

void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c)
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (bd->prev_char != NULL && window == bd->window)
        bd->prev_char(window, c);

    ImGuiIO_AddInputCharacter(io, c);
}

void ImGui_ImplGlfw_MonitorCallback(GLFWmonitor *monitor, int event)
{
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    bd->want_update_monitors = true;
}

void ImGui_ImplGlfw_InstallCallbacks(GLFWwindow* window)
{
	ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
	assert(bd->installed_callbacks == false && "Callbacks already installed!");
	assert(bd->window == window);

	bd->prev_window_focus = glfwSetWindowFocusCallback(window, ImGui_ImplGlfw_WindowFocusCallback);
	bd->prev_cursor_enter = glfwSetCursorEnterCallback(window, ImGui_ImplGlfw_CursorEnterCallback);
	bd->prev_cursor_pos = glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
	bd->prev_mousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
	bd->prev_scroll = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
	bd->prev_key = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
	bd->prev_char = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	bd->prev_monitor = glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);
	bd->installed_callbacks = true;
}

void ImGui_ImplGlfw_RestoreCallbacks(GLFWwindow* window)
{
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    IM_ASSERT(bd->installed_callbacks == true && "Callbacks not installed!");
    IM_ASSERT(bd->window == window);

    glfwSetWindowFocusCallback(window, bd->prev_window_focus);
    glfwSetCursorEnterCallback(window, bd->prev_cursor_enter);
    glfwSetCursorPosCallback(window, bd->prev_cursor_pos);
    glfwSetMouseButtonCallback(window, bd->prev_mousebutton);
    glfwSetScrollCallback(window, bd->prev_scroll);
    glfwSetKeyCallback(window, bd->prev_key);
    glfwSetCharCallback(window, bd->prev_char);
    glfwSetMonitorCallback(bd->prev_monitor);
    bd->installed_callbacks = false;
    bd->prev_window_focus = NULL;
    bd->prev_cursor_enter = NULL;
    bd->prev_cursor_pos = NULL;
    bd->prev_mousebutton = NULL;
    bd->prev_scroll = NULL;
    bd->prev_key = NULL;
    bd->prev_char = NULL;
    bd->prev_monitor = NULL;
}

static bool ImGui_ImplGlfw_Init(GLFWwindow* window, bool install_callbacks, GlfwClientApi client_api)
{
	ImGuiIO *io = igGetIO();

	ImGui_ImplGlfw_Data *bd = IM_NEW(ImGui_ImplGlfw_Data, 1);
    memset(bd, 0, sizeof(*bd));
	io->BackendPlatformUserData = bd;
    io->BackendPlatformName = "imgui_impl_glfw";
	io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io->BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && defined(_WIN32))
    io->BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;
#endif
 
	bd->window = window;
    bd->time = 0.0;
    bd->want_update_monitors = true;
    io->SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
    io->GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
    io->ClipboardUserData = bd->window;     
    // Create mouse cursors    // (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
    // GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
    // Missing cursors will return nullptr and our _UpdateMouseCursor() function will use the Arrow cursor instead.)
	GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
	bd->mouse_cursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	bd->mouse_cursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    bd->mouse_cursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	bd->mouse_cursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    bd->mouse_cursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
#if GLFW_HAS_NEW_CURSORS
    bd->mouse_cursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
    bd->mouse_cursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
	bd->mouse_cursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
	bd->mouse_cursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
#else
	bd->mouse_cursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	bd->mouse_cursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	bd->mouse_cursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	bd->mouse_cursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
#endif
    glfwSetErrorCallback(prev_error_callback);
#if (GLFW_VERSION_COMBINED >= 3300) // Eat errors (see #5785)   
    (void)glfwGetError(NULL);
#endif
 
    if (install_callbacks)
        ImGui_ImplGlfw_InstallCallbacks(window);
 
    // Update monitors the first time (note: monitor callback are broken in GLFW 3.2 and earlier, see github.com/glfw/glfw/issues/784)
    ImGui_ImplGlfw_UpdateMonitors();
    glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);
 
    // Our mouse update function expect PlatformHandle to be filled for the main viewport
    ImGuiViewport* main_viewport = igGetMainViewport();
    main_viewport->PlatformHandle = (void*)bd->window;
#ifdef _WIN32
    main_viewport->PlatformHandleRaw = glfwGetWin32Window(bd->Window);
#elif defined(__APPLE__)
    main_viewport->PlatformHandleRaw = (void*)glfwGetCocoaWindow(bd->Window);
#endif
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        ImGui_ImplGlfw_InitPlatformInterface();
 
    bd->client_api = client_api;
    return true;
}

bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks)
{
    return ImGui_ImplGlfw_Init(window, install_callbacks, GlfwClientApi_OpenGL);
}

void ImGui_ImplGlfw_Shutdown()
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    IM_ASSERT(bd != NULL && "No platform backend to shutdown, or already shutdown?");

    ImGui_ImplGlfw_ShutdownPlatformInterface();

    if (bd->installed_callbacks)
        ImGui_ImplGlfw_RestoreCallbacks(bd->window);

    for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
        glfwDestroyCursor(bd->mouse_cursors[cursor_n]);

    io->BackendPlatformName = NULL;
    io->BackendPlatformUserData = NULL;
    IM_DELETE(bd);
}

static void ImGui_ImplGlfw_UpdateMouseData()
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    ImGuiPlatformIO *platform_io = igGetPlatformIO();

    if (glfwGetInputMode(bd->window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        ImGuiIO_AddMousePosEvent(io, -FLT_MAX, -FLT_MAX);
        return;
    }

    ImGuiID mouse_viewport_id = 0;
    const ImVec2 mouse_pos_prev = io->MousePos;
    for (int n = 0; n < platform_io->Viewports.Size; n++)
    {
        ImGuiViewport* viewport = platform_io->Viewports.Data[n];
        GLFWwindow* window = (GLFWwindow*)viewport->PlatformHandle;

#ifdef __EMSCRIPTEN__
        const bool is_window_focused = true;
#else
        const bool is_window_focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
#endif
        if (is_window_focused)
        {
            // (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
            // When multi-viewports are enabled, all Dear ImGui positions are same as OS positions.
            if (io->WantSetMousePos)
                glfwSetCursorPos(window, (double)(mouse_pos_prev.x - viewport->Pos.x), (double)(mouse_pos_prev.y - viewport->Pos.y));

            // (Optional) Fallback to provide mouse position when focused (ImGui_ImplGlfw_CursorPosCallback already provides this when hovered or captured)
            if (bd->mouse_window == NULL)
            {
                double mouse_x, mouse_y;
                glfwGetCursorPos(window, &mouse_x, &mouse_y);
                if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    // Single viewport mode: mouse position in client window coordinates (io.MousePos is (0,0) when the mouse is on the upper-left corner of the app window)
                    // Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
                    int window_x, window_y;
                    glfwGetWindowPos(window, &window_x, &window_y);
                    mouse_x += window_x;
                    mouse_y += window_y;
                }
                bd->last_valid_mouse_pos = CImVec2((float)mouse_x, (float)mouse_y);
                ImGuiIO_AddMousePosEvent(io, (float)mouse_x, (float)mouse_y);
            }
        }

        // (Optional) When using multiple viewports: call io.AddMouseViewportEvent() with the viewport the OS mouse cursor is hovering.
        // If ImGuiBackendFlags_HasMouseHoveredViewport is not set by the backend, Dear imGui will ignore this field and infer the information using its flawed heuristic.
        // - [X] GLFW >= 3.3 backend ON WINDOWS ONLY does correctly ignore viewports with the _NoInputs flag.
        // - [!] GLFW <= 3.2 backend CANNOT correctly ignore viewports with the _NoInputs flag, and CANNOT reported Hovered Viewport because of mouse capture.
        //       Some backend are not able to handle that correctly. If a backend report an hovered viewport that has the _NoInputs flag (e.g. when dragging a window
        //       for docking, the viewport has the _NoInputs flag in order to allow us to find the viewport under), then Dear ImGui is forced to ignore the value reported
        //       by the backend, and use its flawed heuristic to guess the viewport behind.
        // - [X] GLFW backend correctly reports this regardless of another viewport behind focused and dragged from (we need this to find a useful drag and drop target).
        // FIXME: This is currently only correct on Win32. See what we do below with the WM_NCHITTEST, missing an equivalent for other systems.
        // See https://github.com/glfw/glfw/issues/1236 if you want to help in making this a GLFW feature.
#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && defined(_WIN32))
        const bool window_no_input = (viewport->Flags & ImGuiViewportFlags_NoInputs) != 0;
#if GLFW_HAS_MOUSE_PASSTHROUGH
        glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, window_no_input);
#endif
        if (glfwGetWindowAttrib(window, GLFW_HOVERED) && !window_no_input)
            mouse_viewport_id = viewport->ID;
#else
        // We cannot use bd->MouseWindow maintained from CursorEnter/Leave callbacks, because it is locked to the window capturing mouse.
#endif
    }

    if (io->BackendFlags & ImGuiBackendFlags_HasMouseHoveredViewport)
        ImGuiIO_AddMouseViewportEvent(io, mouse_viewport_id);
}

static void ImGui_ImplGlfw_UpdateMouseCursor()
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if ((io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(bd->window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        return;

    ImGuiMouseCursor imgui_cursor = igGetMouseCursor();
    ImGuiPlatformIO *platform_io = igGetPlatformIO();
    for (int n = 0; n < platform_io->Viewports.Size; n++)
    {
        GLFWwindow* window = (GLFWwindow*)platform_io->Viewports.Data[n]->PlatformHandle;
        if (imgui_cursor == ImGuiMouseCursor_None || io->MouseDrawCursor)
        {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            // Show OS mouse cursor
            // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
            glfwSetCursor(window, bd->mouse_cursors[imgui_cursor] ? bd->mouse_cursors[imgui_cursor] : bd->mouse_cursors[ImGuiMouseCursor_Arrow]);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

// Update gamepad inputs
static inline float Saturate(float v) { return v < 0.0f ? 0.0f : v  > 1.0f ? 1.0f : v; }
static void ImGui_ImplGlfw_UpdateGamepads()
{
    ImGuiIO *io = igGetIO();
    if ((io->ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) // FIXME: Technically feeding gamepad shouldn't depend on this now that they are regular inputs.
        return;

    io->BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
#if GLFW_HAS_GAMEPAD_API
    GLFWgamepadstate gamepad;
    if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad))
        return;
    #define MAP_BUTTON(KEY_NO, BUTTON_NO, _UNUSED)          do { ImGuiIO_AddKeyEvent(io, KEY_NO, gamepad.buttons[BUTTON_NO] != 0); } while (0)
    #define MAP_ANALOG(KEY_NO, AXIS_NO, _UNUSED, V0, V1)    do { float v = gamepad.axes[AXIS_NO]; v = (v - V0) / (V1 - V0); ImGuiIO_AddKeyAnalogEvent(io, KEY_NO, v > 0.10f, Saturate(v)); } while (0)
#else
    int axes_count = 0, buttons_count = 0;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
    if (axes_count == 0 || buttons_count == 0)
        return;
    #define MAP_BUTTON(KEY_NO, _UNUSED, BUTTON_NO)          do { ImGuiIO_AddKeyEvent(io, KEY_NO, (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS)); } while (0)
    #define MAP_ANALOG(KEY_NO, _UNUSED, AXIS_NO, V0, V1)    do { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); ImGuiIO_AddKeyAnalogEvent(io, KEY_NO, v > 0.10f, Saturate(v)); } while (0)
#endif
    io->BackendFlags |= ImGuiBackendFlags_HasGamepad;
    MAP_BUTTON(ImGuiKey_GamepadStart,       GLFW_GAMEPAD_BUTTON_START,          7);
    MAP_BUTTON(ImGuiKey_GamepadBack,        GLFW_GAMEPAD_BUTTON_BACK,           6);
    MAP_BUTTON(ImGuiKey_GamepadFaceLeft,    GLFW_GAMEPAD_BUTTON_X,              2);     // Xbox X, PS Square
    MAP_BUTTON(ImGuiKey_GamepadFaceRight,   GLFW_GAMEPAD_BUTTON_B,              1);     // Xbox B, PS Circle
    MAP_BUTTON(ImGuiKey_GamepadFaceUp,      GLFW_GAMEPAD_BUTTON_Y,              3);     // Xbox Y, PS Triangle
    MAP_BUTTON(ImGuiKey_GamepadFaceDown,    GLFW_GAMEPAD_BUTTON_A,              0);     // Xbox A, PS Cross
    MAP_BUTTON(ImGuiKey_GamepadDpadLeft,    GLFW_GAMEPAD_BUTTON_DPAD_LEFT,      13);
    MAP_BUTTON(ImGuiKey_GamepadDpadRight,   GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,     11);
    MAP_BUTTON(ImGuiKey_GamepadDpadUp,      GLFW_GAMEPAD_BUTTON_DPAD_UP,        10);
    MAP_BUTTON(ImGuiKey_GamepadDpadDown,    GLFW_GAMEPAD_BUTTON_DPAD_DOWN,      12);
    MAP_BUTTON(ImGuiKey_GamepadL1,          GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,    4);
    MAP_BUTTON(ImGuiKey_GamepadR1,          GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,   5);
    MAP_ANALOG(ImGuiKey_GamepadL2,          GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,     4,      -0.75f,  +1.0f);
    MAP_ANALOG(ImGuiKey_GamepadR2,          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,    5,      -0.75f,  +1.0f);
    MAP_BUTTON(ImGuiKey_GamepadL3,          GLFW_GAMEPAD_BUTTON_LEFT_THUMB,     8);
    MAP_BUTTON(ImGuiKey_GamepadR3,          GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,    9);
    MAP_ANALOG(ImGuiKey_GamepadLStickLeft,  GLFW_GAMEPAD_AXIS_LEFT_X,           0,      -0.25f,  -1.0f);
    MAP_ANALOG(ImGuiKey_GamepadLStickRight, GLFW_GAMEPAD_AXIS_LEFT_X,           0,      +0.25f,  +1.0f);
    MAP_ANALOG(ImGuiKey_GamepadLStickUp,    GLFW_GAMEPAD_AXIS_LEFT_Y,           1,      -0.25f,  -1.0f);
    MAP_ANALOG(ImGuiKey_GamepadLStickDown,  GLFW_GAMEPAD_AXIS_LEFT_Y,           1,      +0.25f,  +1.0f);
    MAP_ANALOG(ImGuiKey_GamepadRStickLeft,  GLFW_GAMEPAD_AXIS_RIGHT_X,          2,      -0.25f,  -1.0f);
    MAP_ANALOG(ImGuiKey_GamepadRStickRight, GLFW_GAMEPAD_AXIS_RIGHT_X,          2,      +0.25f,  +1.0f);
    MAP_ANALOG(ImGuiKey_GamepadRStickUp,    GLFW_GAMEPAD_AXIS_RIGHT_Y,          3,      -0.25f,  -1.0f);
    MAP_ANALOG(ImGuiKey_GamepadRStickDown,  GLFW_GAMEPAD_AXIS_RIGHT_Y,          3,      +0.25f,  +1.0f);
    #undef MAP_BUTTON
    #undef MAP_ANALOG
}

static void ImGui_ImplGlfw_UpdateMonitors()
{
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    ImGuiPlatformIO *platform_io = igGetPlatformIO();
    int monitors_count = 0;
    GLFWmonitor** glfw_monitors = glfwGetMonitors(&monitors_count);
    platform_io->Monitors.Data = IM_NEW(ImGuiPlatformMonitor, monitors_count);
    platform_io->Monitors.Capacity = monitors_count;
    platform_io->Monitors.Size = 0;

    for (int n = 0; n < monitors_count; n++)
    {
        ImGuiPlatformMonitor monitor;
        int x, y;
        glfwGetMonitorPos(glfw_monitors[n], &x, &y);
        const GLFWvidmode* vid_mode = glfwGetVideoMode(glfw_monitors[n]);
        monitor.MainPos = monitor.WorkPos = CImVec2((float)x, (float)y);
        monitor.MainSize = monitor.WorkSize = CImVec2((float)vid_mode->width, (float)vid_mode->height);
#if GLFW_HAS_MONITOR_WORK_AREA
        int w, h;
        glfwGetMonitorWorkarea(glfw_monitors[n], &x, &y, &w, &h);
        if (w > 0 && h > 0) // Workaround a small GLFW issue reporting zero on monitor changes: https://github.com/glfw/glfw/pull/1761
        {
            monitor.WorkPos = CImVec2((float)x, (float)y);
            monitor.WorkSize = CImVec2((float)w, (float)h);
        }
#endif
#if GLFW_HAS_PER_MONITOR_DPI
        // Warning: the validity of monitor DPI information on Windows depends on the application DPI awareness settings, which generally needs to be set in the manifest or at runtime.
        float x_scale, y_scale;
        glfwGetMonitorContentScale(glfw_monitors[n], &x_scale, &y_scale);
        monitor.DpiScale = x_scale;
#endif
        platform_io->Monitors.Data[platform_io->Monitors.Size++] = monitor;
    }
    bd->want_update_monitors = false;
}

void ImGui_ImplGlfw_NewFrame()
{
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    IM_ASSERT(bd != NULL && "Did you call ImGui_ImplGlfw_InitForXXX()?");

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(bd->window, &w, &h);
    glfwGetFramebufferSize(bd->window, &display_w, &display_h);
    io->DisplaySize = CImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io->DisplayFramebufferScale = CImVec2((float)display_w / (float)w, (float)display_h / (float)h);
    if (bd->want_update_monitors)
        ImGui_ImplGlfw_UpdateMonitors();

    // Setup time step
    double current_time = glfwGetTime();
    io->DeltaTime = bd->time > 0.0 ? (float)(current_time - bd->time) : (float)(1.0f / 60.0f);
    bd->time = current_time;

    ImGui_ImplGlfw_UpdateMouseData();
    ImGui_ImplGlfw_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    ImGui_ImplGlfw_UpdateGamepads();
}

//--------------------------------------------------------------------------------------------------------
// MULTI-VIEWPORT / PLATFORM INTERFACE SUPPORT
// This is an _advanced_ and _optional_ feature, allowing the backend to create and handle multiple viewports simultaneously.
// If you are new to dear imgui or creating a new binding for dear imgui, it is recommended that you completely ignore this section first..
//--------------------------------------------------------------------------------------------------------

// Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
typedef struct ImGui_ImplGlfw_ViewportData
{
    GLFWwindow* Window;
    bool        WindowOwned;
    int         IgnoreWindowPosEventFrame;
    int         IgnoreWindowSizeEventFrame;
} ImGui_ImplGlfw_ViewportData;

static void ImGui_ImplGlfw_WindowCloseCallback(GLFWwindow *window)
{
    ImGuiViewport *viewport;
    if ((viewport = igFindViewportByPlatformHandle(window)))
        viewport->PlatformRequestClose = true;
}

// GLFW may dispatch window pos/size events after calling glfwSetWindowPos()/glfwSetWindowSize().
// However: depending on the platform the callback may be invoked at different time:
// - on Windows it appears to be called within the glfwSetWindowPos()/glfwSetWindowSize() call
// - on Linux it is queued and invoked during glfwPollEvents()
// Because the event doesn't always fire on glfwSetWindowXXX() we use a frame counter tag to only
// ignore recent glfwSetWindowXXX() calls.
static void ImGui_ImplGlfw_WindowPosCallback(GLFWwindow *window, int _, int __)
{
    ImGuiViewport *viewport;
    if ((viewport = igFindViewportByPlatformHandle(window)))
    {
        ImGui_ImplGlfw_ViewportData *vd;
        if ((vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData))
        {
            bool ignore_event = (igGetFrameCount() <= vd->IgnoreWindowPosEventFrame + 1);
            //data->IgnoreWindowPosEventFrame = -1;
            if (ignore_event)
                return;
        }
        viewport->PlatformRequestMove = true;
    }
}

static void ImGui_ImplGlfw_WindowSizeCallback(GLFWwindow* window, int _, int __)
{
    ImGuiViewport *viewport;
    if ((viewport = igFindViewportByPlatformHandle(window)))
    {
        ImGui_ImplGlfw_ViewportData *vd;
        if ((vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData))
        {
            bool ignore_event = (igGetFrameCount() <= vd->IgnoreWindowSizeEventFrame + 1);
            //data->IgnoreWindowSizeEventFrame = -1;
            if (ignore_event)
                return;
        }
        viewport->PlatformRequestResize = true;
    }
}

static void ImGui_ImplGlfw_CreateWindow(ImGuiViewport* viewport)
{
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    ImGui_ImplGlfw_ViewportData *vd = IM_NEW(ImGui_ImplGlfw_ViewportData, 1);
    vd->Window = NULL;
    vd->WindowOwned = false;
    vd->IgnoreWindowPosEventFrame = -1;
    vd->IgnoreWindowSizeEventFrame = -1;
	memset(vd, 0, sizeof(ImGui_ImplGlfw_ViewportData));
    viewport->PlatformUserData = vd;

    // GLFW 3.2 unfortunately always set focus on glfwCreateWindow() if GLFW_VISIBLE is set, regardless of GLFW_FOCUSED
    // With GLFW 3.3, the hint GLFW_FOCUS_ON_SHOW fixes this problem
    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindowHint(GLFW_FOCUSED, false);
#if GLFW_HAS_FOCUS_ON_SHOW
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, false);
 #endif
    glfwWindowHint(GLFW_DECORATED, (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? false : true);
#if GLFW_HAS_WINDOW_TOPMOST
    glfwWindowHint(GLFW_FLOATING, (viewport->Flags & ImGuiViewportFlags_TopMost) ? true : false);
#endif
    GLFWwindow* share_window = (bd->client_api == GlfwClientApi_OpenGL) ? bd->window : NULL;
    vd->Window = glfwCreateWindow((int)viewport->Size.x, (int)viewport->Size.y, "No Title Yet", NULL, share_window);
    vd->WindowOwned = true;
    viewport->PlatformHandle = (void*)vd->Window;
#ifdef _WIN32
    viewport->PlatformHandleRaw = glfwGetWin32Window(vd->Window);
#elif defined(__APPLE__)
    viewport->PlatformHandleRaw = (void*)glfwGetCocoaWindow(vd->Window);
#endif
    glfwSetWindowPos(vd->Window, (int)viewport->Pos.x, (int)viewport->Pos.y);

    // Install GLFW callbacks for secondary viewports
    glfwSetWindowFocusCallback(vd->Window, ImGui_ImplGlfw_WindowFocusCallback);
    glfwSetCursorEnterCallback(vd->Window, ImGui_ImplGlfw_CursorEnterCallback);
    glfwSetCursorPosCallback(vd->Window, ImGui_ImplGlfw_CursorPosCallback);
    glfwSetMouseButtonCallback(vd->Window, ImGui_ImplGlfw_MouseButtonCallback);
    glfwSetScrollCallback(vd->Window, ImGui_ImplGlfw_ScrollCallback);
    glfwSetKeyCallback(vd->Window, ImGui_ImplGlfw_KeyCallback);
    glfwSetCharCallback(vd->Window, ImGui_ImplGlfw_CharCallback);
    glfwSetWindowCloseCallback(vd->Window, ImGui_ImplGlfw_WindowCloseCallback);
    glfwSetWindowPosCallback(vd->Window, ImGui_ImplGlfw_WindowPosCallback);
    glfwSetWindowSizeCallback(vd->Window, ImGui_ImplGlfw_WindowSizeCallback);
    if (bd->client_api == GlfwClientApi_OpenGL)
    {
        glfwMakeContextCurrent(vd->Window);
        glfwSwapInterval(0);
    }
}

static void ImGui_ImplGlfw_DestroyWindow(ImGuiViewport* viewport)
{
    ImGui_ImplGlfw_Data *bd = ImGui_ImplGlfw_GetBackendData();
    ImGui_ImplGlfw_ViewportData *vd;
    if ((vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData))
    {
        if (vd->WindowOwned)
        {
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && defined(_WIN32)
            HWND hwnd = (HWND)viewport->PlatformHandleRaw;
            ::RemovePropA(hwnd, "IMGUI_VIEWPORT");
#endif

            // Release any keys that were pressed in the window being destroyed and are still held down,
            // because we will not receive any release events after window is destroyed.
            for (int i = 0; i < IM_ARRAYSIZE(bd->key_owner_windows); i++)
                if (bd->key_owner_windows[i] == vd->Window)
                    ImGui_ImplGlfw_KeyCallback(vd->Window, i, 0, GLFW_RELEASE, 0); // Later params are only used for main viewport, on which this function is never called.

            glfwDestroyWindow(vd->Window);
        }
        vd->Window = NULL;
        IM_DELETE(vd);
    }
    viewport->PlatformUserData = viewport->PlatformHandle = NULL;
}

// We have submitted https://github.com/glfw/glfw/pull/1568 to allow GLFW to support "transparent inputs".
// In the meanwhile we implement custom per-platform workarounds here (FIXME-VIEWPORT: Implement same work-around for Linux/OSX!)
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && defined(_WIN32)
static LRESULT CALLBACK WndProcNoInputs(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    if (msg == WM_NCHITTEST)
    {
        // Let mouse pass-through the window. This will allow the backend to call io.AddMouseViewportEvent() properly (which is OPTIONAL).
        // The ImGuiViewportFlags_NoInputs flag is set while dragging a viewport, as want to detect the window behind the one we are dragging.
        // If you cannot easily access those viewport flags from your windowing/event code: you may manually synchronize its state e.g. in
        // your main loop after calling UpdatePlatformWindows(). Iterate all viewports/platform windows and pass the flag to your windowing system.
        ImGuiViewport* viewport = (ImGuiViewport*)::GetPropA(hWnd, "IMGUI_VIEWPORT");
        if (viewport->Flags & ImGuiViewportFlags_NoInputs)
            return HTTRANSPARENT;
    }
    return ::CallWindowProc(bd->GlfwWndProc, hWnd, msg, wParam, lParam);
}
#endif

static void ImGui_ImplGlfw_ShowWindow(ImGuiViewport* viewport)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;

#if defined(_WIN32)
    // GLFW hack: Hide icon from task bar
    HWND hwnd = (HWND)viewport->PlatformHandleRaw;
    if (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
    {
        LONG ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
        ex_style &= ~WS_EX_APPWINDOW;
        ex_style |= WS_EX_TOOLWINDOW;
        ::SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);
    }

    // GLFW hack: install hook for WM_NCHITTEST message handler
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && defined(_WIN32)
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    ::SetPropA(hwnd, "IMGUI_VIEWPORT", viewport);
    if (bd->GlfwWndProc == nullptr)
        bd->GlfwWndProc = (WNDPROC)::GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    ::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProcNoInputs);
#endif

#if !GLFW_HAS_FOCUS_ON_SHOW
    // GLFW hack: GLFW 3.2 has a bug where glfwShowWindow() also activates/focus the window.
    // The fix was pushed to GLFW repository on 2018/01/09 and should be included in GLFW 3.3 via a GLFW_FOCUS_ON_SHOW window attribute.
    // See https://github.com/glfw/glfw/issues/1189
    // FIXME-VIEWPORT: Implement same work-around for Linux/OSX in the meanwhile.
    if (viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
    {
        ::ShowWindow(hwnd, SW_SHOWNA);
        return;
    }
#endif
#endif

    glfwShowWindow(vd->Window);
}

static ImVec2 ImGui_ImplGlfw_GetWindowPos(ImGuiViewport* viewport)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    int x = 0, y = 0;
    glfwGetWindowPos(vd->Window, &x, &y);
    return CImVec2((float)x, (float)y);
}

static void ImGui_ImplGlfw_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    vd->IgnoreWindowPosEventFrame = igGetFrameCount();
    glfwSetWindowPos(vd->Window, (int)pos.x, (int)pos.y);
}

static ImVec2 ImGui_ImplGlfw_GetWindowSize(ImGuiViewport* viewport)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    int w = 0, h = 0;
    glfwGetWindowSize(vd->Window, &w, &h);
    return CImVec2((float)w, (float)h);
}

static void ImGui_ImplGlfw_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
#if __APPLE__ && !GLFW_HAS_OSX_WINDOW_POS_FIX
    // Native OS windows are positioned from the bottom-left corner on macOS, whereas on other platforms they are
    // positioned from the upper-left corner. GLFW makes an effort to convert macOS style coordinates, however it
    // doesn't handle it when changing size. We are manually moving the window in order for changes of size to be based
    // on the upper-left corner.
    int x, y, width, height;
    glfwGetWindowPos(vd->Window, &x, &y);
    glfwGetWindowSize(vd->Window, &width, &height);
    glfwSetWindowPos(vd->Window, x, y - height + size.y);
#endif
    vd->IgnoreWindowSizeEventFrame = igGetFrameCount();
    glfwSetWindowSize(vd->Window, (int)size.x, (int)size.y);
}

static void ImGui_ImplGlfw_SetWindowTitle(ImGuiViewport* viewport, const char* title)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    glfwSetWindowTitle(vd->Window, title);
}

static void ImGui_ImplGlfw_SetWindowFocus(ImGuiViewport* viewport)
{
#if GLFW_HAS_FOCUS_WINDOW
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    glfwFocusWindow(vd->Window);
#else
    // FIXME: What are the effect of not having this function? At the moment imgui doesn't actually call SetWindowFocus - we set that up ahead, will answer that question later.
    (void)viewport;
#endif
}

static bool ImGui_ImplGlfw_GetWindowFocus(ImGuiViewport* viewport)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    return glfwGetWindowAttrib(vd->Window, GLFW_FOCUSED) != 0;
}

static bool ImGui_ImplGlfw_GetWindowMinimized(ImGuiViewport* viewport)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    return glfwGetWindowAttrib(vd->Window, GLFW_ICONIFIED) != 0;
}

#if GLFW_HAS_WINDOW_ALPHA
static void ImGui_ImplGlfw_SetWindowAlpha(ImGuiViewport* viewport, float alpha)
{
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    glfwSetWindowOpacity(vd->Window, alpha);
}
#endif

static void ImGui_ImplGlfw_RenderWindow(ImGuiViewport* viewport, void* _)
{
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    if (bd->client_api == GlfwClientApi_OpenGL)
        glfwMakeContextCurrent(vd->Window);
}

static void ImGui_ImplGlfw_SwapBuffers(ImGuiViewport* viewport, void* _)
{
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    ImGui_ImplGlfw_ViewportData* vd = (ImGui_ImplGlfw_ViewportData*)viewport->PlatformUserData;
    if (bd->client_api == GlfwClientApi_OpenGL)
    {
        glfwMakeContextCurrent(vd->Window);
        glfwSwapBuffers(vd->Window);
    }
}
static void ImGui_ImplGlfw_InitPlatformInterface()
{
    // Register platform interface (will be coupled with a renderer interface)
    ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
    ImGuiPlatformIO* platform_io = igGetPlatformIO();
    platform_io->Platform_CreateWindow = ImGui_ImplGlfw_CreateWindow;
    platform_io->Platform_DestroyWindow = ImGui_ImplGlfw_DestroyWindow;
    platform_io->Platform_ShowWindow = ImGui_ImplGlfw_ShowWindow;
    platform_io->Platform_SetWindowPos = ImGui_ImplGlfw_SetWindowPos;
    platform_io->Platform_GetWindowPos = ImGui_ImplGlfw_GetWindowPos;
    platform_io->Platform_SetWindowSize = ImGui_ImplGlfw_SetWindowSize;
    platform_io->Platform_GetWindowSize = ImGui_ImplGlfw_GetWindowSize;
    platform_io->Platform_SetWindowFocus = ImGui_ImplGlfw_SetWindowFocus;
    platform_io->Platform_GetWindowFocus = ImGui_ImplGlfw_GetWindowFocus;
    platform_io->Platform_GetWindowMinimized = ImGui_ImplGlfw_GetWindowMinimized;
    platform_io->Platform_SetWindowTitle = ImGui_ImplGlfw_SetWindowTitle;
    platform_io->Platform_RenderWindow = ImGui_ImplGlfw_RenderWindow;
    platform_io->Platform_SwapBuffers = ImGui_ImplGlfw_SwapBuffers;
#if GLFW_HAS_WINDOW_ALPHA
    platform_io->Platform_SetWindowAlpha = ImGui_ImplGlfw_SetWindowAlpha;
#endif

    // Register main window handle (which is owned by the main application, not by us)
    // This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
    ImGuiViewport* main_viewport = igGetMainViewport();
    ImGui_ImplGlfw_ViewportData* vd = IM_NEW(ImGui_ImplGlfw_ViewportData, 1);
    vd->Window = bd->window;
    vd->WindowOwned = false;
    main_viewport->PlatformUserData = vd;
    main_viewport->PlatformHandle = (void*)bd->window;
}

static void ImGui_ImplGlfw_ShutdownPlatformInterface()
{
    igDestroyPlatformWindows();
}
