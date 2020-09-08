workspace "Motoret"
	configurations {"Debug", "Release"}
    location "build"

project "motoret"
	kind "ConsoleApp"
    language "C++"
    targetdir "bin/"
    objdir "buid/"
	files {
		"src/*.h",
		"src/*.cpp",
        "src/*/*.h",
        "src/*/*.cpp",
	}

    filter "configurations:Debug"
        defines "MEREN_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "MEREN_RELEASE"
        optimize "On"

