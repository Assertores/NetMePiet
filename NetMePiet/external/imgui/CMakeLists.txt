cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(imgui C CXX)

set(CPP_FILES
	"imgui/imgui.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_widgets.cpp"
	"imgui/imgui_demo.cpp"

	"imgui/misc/cpp/imgui_stdlib.cpp"

	#"imgui_plot_var.cpp"

	"imgui/examples/imgui_impl_sdl.cpp"
	"imgui/examples/imgui_impl_opengl3.cpp"
)

set(HPP_FILES
	"imgui/imgui.h"
	"imgui/imstb_rectpack.h"
	"imgui/imstb_textedit.h"
	"imgui/imstb_truetype.h"

	"imgui/misc/cpp/imgui_stdlib.h"

	#"imgui_plot_var.hpp"

	"imgui/examples/imgui_impl_sdl.h"
	"imgui/examples/imgui_impl_opengl3.h"
)

add_library(imgui ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
#target_include_directories(imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui") # im sad

if (UNIX)
	target_include_directories(imgui PUBLIC "${SDL2_INCLUDE_DIRS}")
endif()

target_link_libraries(imgui glad)

#if (UNIX)
	#target_link_libraries(imgui ${SDL2_LIBRARY})
#else()
target_link_libraries(imgui SDL2)
#endif()

