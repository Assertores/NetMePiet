cmake_minimum_required(VERSION 3.2 FATAL_ERROR)


set(TARGET_NAME NetMePiet)

project(${TARGET_NAME} CXX C)

include(settings.cmake)

add_executable(${TARGET_NAME}
	"src/main.cpp"
)

target_sources(${TARGET_NAME}
PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_base.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_chat.hpp"
PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_base.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_chat.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/network_example.cpp"
)

target_include_directories(${TARGET_NAME}
PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

find_package(SDL2 CONFIG REQUIRED)
#find_package(SDL2_net CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)

include("external/concurrentqueue.cmake")


#===== ===== SDL2 hack ===== =====
set_property(TARGET SDL2::SDL2 PROPERTY IMPORTED_GLOBAL True)
add_library(SDL2 ALIAS SDL2::SDL2)

set_property(TARGET SDL2::SDL2main PROPERTY IMPORTED_GLOBAL True)
add_library(SDL2main ALIAS SDL2::SDL2main)

set_property(TARGET SDL2::SDL2_net PROPERTY IMPORTED_GLOBAL True)
add_library(SDL2_net ALIAS SDL2::SDL2_net)
#===== ===== end SDL2 hack ===== =====

target_link_libraries(${TARGET_NAME} PRIVATE
	SDL2
	SDL2main
	SDL2_net

	concurrentqueue
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

