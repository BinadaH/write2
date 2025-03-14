cmake_minimum_required (VERSION 3.28)
project (wapp)


#Compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(deps/sdl/src)
add_subdirectory(deps/imgui)

file(GLOB wapp_src
	"wapp/src/*.cpp"
	"wapp/include/*h"
)


find_package(OpenGL REQUIRED)

add_executable(wapp ${wapp_src})
target_link_libraries(wapp SDL3::SDL3 imgui OpenGL::OpenGL)
target_include_directories(wapp PRIVATE ${CMAKE_SOURCE_DIR}/wapp/include ${CMAKE_SOURCE_DIR}/deps/sdl/include ${CMAKE_SOURCE_DIR}/deps/imgui)
