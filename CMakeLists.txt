cmake_minimum_required(VERSION 3.13)

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(THE_DEMO the-demo)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(THE_DEMO ${THE_DEMO}${CMAKE_DEBUG_POSTFIX})
endif()

project(THE_Demo VERSION 0.0.1)

add_subdirectory(the)

add_executable(${THE_DEMO} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

set_target_properties(${THE_DEMO} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_compile_options(${THE_DEMO} PUBLIC
	-Wall
	-Wpedantic
	)

target_include_directories(${THE_DEMO} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/the/src
	${CMAKE_CURRENT_SOURCE_DIR}/the/extern/include
	)

target_compile_definitions(${THE_DEMO} PUBLIC
	"-DIMGUI_IMPL_API=extern \"C\" "
	"-DCIMGUI_USE_GLFW"
	"-DCIMGUI_USE_OPENGL3"
	)

#target_link_options(${THE_DEMO} PRIVATE -lstdc++)

target_link_libraries(${THE_DEMO} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/the/extern/cimgui/libcimgui$<$<CONFIG:Debug>:${CMAKE_DEBUG_POSTFIX}>.so
	the
	GL
	X11
	dl
	pthread
	m
	)

