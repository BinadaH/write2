file(GLOB wapp_src
	"*.cpp"
	"*h"
)

add_library(imgui ${wapp_src})
target_include_directories(imgui PRIVATE ${CMAKE_SOURCE_DIR}/deps/imgui ${CMAKE_SOURCE_DIR}/deps/sdl/include)

