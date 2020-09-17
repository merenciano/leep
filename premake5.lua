workspace "Motoret"
	configurations {"Debug", "Release"}
    location "build"
    architecture "x86_64"

project "motoret"
	kind "ConsoleApp"
    language "C++"
    targetdir "bin/"
    objdir "build/"
    includedirs {
        "include/",
        "extern/include/",
    }

	files {
        "include/*.h",
		"src/*.h",
		"src/*.cpp",
        "src/*/*.h",
        "src/*/*.cpp",

        --GLFW
        "./extern/src/GLFW/internal.h",
        "./extern/src/GLFW/mappings.h",
        "./extern/src/GLFW/egl_context.h",
        "./extern/src/GLFW/osmesa_context.h",
        "./extern/src/GLFW/context.c",
        "./extern/src/GLFW/init.c",
        "./extern/src/GLFW/input.c",
        "./extern/src/GLFW/monitor.c",
        "./extern/src/GLFW/vulkan.c",
        "./extern/src/GLFW/window.c",
        "./extern/src/GLFW/egl_context.c",
        "./extern/src/GLFW/osmesa_context.c",

        --glad
        "./extern/src/glad.c",

    }
    
    configuration "vs2019"
        defines {"_CRT_SECURE_NO_WARNINGS", "_GLFW_WIN32", "MOTORET_OPENGL", }

        files {
            "./extern/src/GLFW/win32_platform.h",
            "./extern/src/GLFW/win32_joystick.h",
            "./extern/src/GLFW/wgl_context.h",
            "./extern/src/GLFW/win32_init.c",
            "./extern/src/GLFW/win32_joystick.c",
            "./extern/src/GLFW/win32_monitor.c",
            "./extern/src/GLFW/win32_time.c",
            "./extern/src/GLFW/win32_thread.c",
            "./extern/src/GLFW/win32_window.c",
            "./extern/src/GLFW/wgl_context.c",
        }

    configuration "gmake"
        defines {}

        files {}

    filter "configurations:Debug"
        defines "MOTORET_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "MOTORET_RELEASE"
        optimize "On"


