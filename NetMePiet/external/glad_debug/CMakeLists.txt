cmake_minimum_required(VERSION 3.2)
project(glad CXX)

set(CPP_FILES
	"src/glad.c"
)

set(HPP_FILES
	"include/glad/glad.h"
	"include/KHR/khrplatform.h"
)

add_library(glad ${CPP_FILES} ${HPP_FILES})

target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(UNIX)
	target_link_libraries(glad
		dl
	)
endif()

