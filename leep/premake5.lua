
project "leep"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "lib/"
    objdir "build/"
    includedirs {
        "src/",
        "extern/include/",
        "src/extern/"
    }

    defines { "LEEP_" .. _OPTIONS["graphics-api"] }

    files {
        "src/*.h",
        "src/*.cpp",
        "src/*/*.h",
        "src/*/*.cpp",
        "src/*/*/*.h",
        "src/*/*/*.cpp",

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

        --minitrace
        "./extern/src/minitrace.c",

        --imgui
        "./extern/src/imgui/*.cpp",

        --lua
        "./extern/src/lua/**",


    }
    
    configuration "vs2019"
        defines {"_CRT_SECURE_NO_WARNINGS", "_GLFW_WIN32", "LEEP_WIN", }

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

    configuration "vs2017"
        defines {"_CRT_SECURE_NO_WARNINGS", "_GLFW_WIN32", "LEEP_WIN", }

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
        defines { "_GLFW_X11", "LUA_USE_POSIX", "LEEP_LINUX", }
        links { "X11", "dl", "pthread"  }
        buildoptions { "-Wall",  }

        files {
            "./extern/src/GLFW/x11_platform.h",
            "./extern/src/GLFW/xkb_unicode.h",
            "./extern/src/GLFW/posix_time.h",
            "./extern/src/GLFW/posix_thread.h",
            "./extern/src/GLFW/glx_context.h",
            "./extern/src/GLFW/linux_joystick.h",
            "./extern/src/GLFW/x11_init.c",
            "./extern/src/GLFW/x11_monitor.c",
            "./extern/src/GLFW/x11_window.c",
            "./extern/src/GLFW/xkb_unicode.c",
            "./extern/src/GLFW/posix_time.c",
            "./extern/src/GLFW/posix_thread.c",
            "./extern/src/GLFW/glx_context.c",
            "./extern/src/GLFW/linux_joystick.c",
        }

    filter { "files:**/api-impl/*.cpp" }
      flags {"ExcludeFromBuild"}

    filter "configurations:Debug"
        defines { "LEEP_DEBUG" }
        targetsuffix "-d"
        symbols "On"
    
    filter "configurations:Release"
        defines "LEEP_RELEASE"
        optimize "Speed"
        --optimize "Size"

    filter "configurations:Trace"
        defines { "LEEP_RELEASE", "MTR_ENABLED" }
        optimize "On"

