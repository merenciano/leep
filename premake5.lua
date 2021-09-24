newoption {
    trigger = "graphics-api",
    value = "API",
    description = "Choose the 3D graphics backend",
    default = "OPENGL",
    allowed = {
        { "OPENGL", "OpenGL 3.3" },
        { "OPENGL_ES",  "OpenGL for Embedded Systems 3.0" }
    }
}

workspace "TestApp"
    configurations {"Debug", "Release", "Trace" }
    location "build"
    architecture "x86_64"

    include "leep"

project "TestApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    links "leep"
    targetdir "bin/"
    objdir "build/"
    includedirs {
        "leep/src/",
        "leep/src/**",
        "src/",
        "src/**",
    }
    libdirs {
        "leep/lib",
    }

    files {
        "./src/**.cpp"
    }

    configuration "vs2019"
        defines {"_CRT_SECURE_NO_WARNINGS", "LEEP_WIN", }
    configuration "gmake"
        defines { "LEEP_LINUX", }
        links { "X11", "dl", "pthread"  }
    
    filter "configurations:Debug"
        defines { "LEEP_DEBUG", }
        targetsuffix "-d"
        symbols "On"
    
    filter "configurations:Release"
        optimize "On"

