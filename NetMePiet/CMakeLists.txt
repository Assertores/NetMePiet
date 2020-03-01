cmake_minimum_required(VERSION 3.2 FATAL_ERROR)


set(TARGET_NAME NetMePiet)

project(${TARGET_NAME} CXX C)

include(settings.cmake)

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

#===== librarys =====
add_library(NMP_messages
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_base.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_chat.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_pixel.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_selected.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_size.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/messages/message_status.hpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_base.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_chat.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_pixel.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_selected.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_size.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/message_status.cpp"
)

target_include_directories(NMP_messages
PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(NMP_messages PUBLIC cxx_std_17)

add_library(NMP_networkAPI
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/netcode_api.hpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/netcode_api.cpp"
)

target_include_directories(NMP_networkAPI
PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(NMP_networkAPI PRIVATE
	SDL2
	SDL2_net
	concurrentqueue

	NMP_messages
)

add_executable(NMP_network_test
	"src/network_example.cpp"
)

target_link_libraries(NMP_network_test PRIVATE
	SDL2
	SDL2main
	SDL2_net
	concurrentqueue

	NMP_messages
	NMP_networkAPI
)

add_executable(${TARGET_NAME}
	"src/main.cpp"
)

target_include_directories(${TARGET_NAME}
PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${TARGET_NAME} PRIVATE
	SDL2
	SDL2main
	SDL2_net
	concurrentqueue

	NMP_messages
	NMP_networkAPI
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

