cmake_minimum_required(VERSION 3.2 FATAL_ERROR)


set(TARGET_NAME NetMePiet)

project(${TARGET_NAME} CXX C)

include(settings.cmake)

find_package(SDL2 CONFIG REQUIRED)
if (UNIX)
	find_package(PkgConfig REQUIRED)

	#find_package(SDL2_net CONFIG REQUIRED)
	pkg_check_modules(SDL2_net REQUIRED SDL2_net)
else(UNIX)
	find_package(sdl2-net CONFIG REQUIRED)
endif()

include("external/concurrentqueue.cmake")

add_subdirectory("external/glad_debug") # TODO: use normal glad for non debug
add_subdirectory("external/imgui")

if (NOT UNIX)
	#===== ===== SDL2 hack ===== =====
	set_property(TARGET SDL2::SDL2 PROPERTY IMPORTED_GLOBAL True)
	add_library(SDL2 ALIAS SDL2::SDL2)

	set_property(TARGET SDL2::SDL2main PROPERTY IMPORTED_GLOBAL True)
	add_library(SDL2main ALIAS SDL2::SDL2main)

	set_property(TARGET SDL2::SDL2_net PROPERTY IMPORTED_GLOBAL True)
	add_library(SDL2_net ALIAS SDL2::SDL2_net)
	#===== ===== end SDL2 hack ===== =====
endif()

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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/netcode_helper.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/network/netcode_threads.hpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/netcode_api.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/netcode_helper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/netcode_threads.cpp"
)

target_include_directories(NMP_networkAPI
PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if (UNIX)
	target_include_directories(NMP_networkAPI PUBLIC
		${SDL2_net_INCLUDE_DIRS}
	)
endif(UNIX)

target_link_libraries(NMP_networkAPI
	SDL2
	concurrentqueue

	NMP_messages
)

if (UNIX)
	target_link_libraries(NMP_networkAPI
		${SDL2_net_LIBRARIES}
		pthread # TODO: why??
	)
else(UNIX)
	target_link_libraries(NMP_networkAPI
		SDL2_net
	)
endif(UNIX)

add_executable(NMP_network_test
	"src/network_example.cpp"
)

target_link_libraries(NMP_network_test PRIVATE
	SDL2
	SDL2main
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

add_executable(NetMePiet_Server
	"src/server_main.cpp"
)

target_include_directories(NetMePiet_Server
PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(NetMePiet_Server PRIVATE
	SDL2
	SDL2main
	SDL2_net
	concurrentqueue

	NMP_messages
	NMP_networkAPI
)

target_compile_features(NetMePiet_Server PUBLIC cxx_std_17)

