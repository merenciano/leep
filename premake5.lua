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
        "leep/src",
    }
    libdirs {
        "leep/lib",
    }

    files "./src/main.cpp"
    
    filter "configurations:Debug"
        targetsuffix "-d"
        symbols "On"
    
    filter "configurations:Release"
        optimize "On"

