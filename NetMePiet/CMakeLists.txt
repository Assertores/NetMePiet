cmake_minimum_required(VERSION 3.2 FATAL_ERROR)


set(TARGET_NAME CPixelPusher)

project(${TARGET_NAME} CXX C)

include(settings.cmake)

add_executable(${TARGET_NAME}
	"src/main.cpp"
)

target_sources(${TARGET_NAME}
PUBLIC
	#"includes/Stuff.hpp"
PRIVATE
	#"src/Stuff.hpp"
)

target_include_directories(${TARGET_NAME}
PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)

target_link_libraries(${TARGET_NAME} PRIVATE
	SDL2::SDL2
	SDL2::SDL2main
	SDL2::SDL2_net
	unofficial::concurrentqueue::concurrentqueue
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
