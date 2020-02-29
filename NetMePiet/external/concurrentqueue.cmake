cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_library(concurrentqueue INTERFACE
	#"${CMAKE_CURRENT_LIST_DIR}/concurrentqueue/concurrentqueue.h"

	#"${CMAKE_CURRENT_LIST_DIR}/concurrentqueue/lightweightsemaphore.h"
	#"${CMAKE_CURRENT_LIST_DIR}/concurrentqueue/blockingconcurrentqueue.h"
)

target_include_directories(concurrentqueue INTERFACE "${CMAKE_CURRENT_LIST_DIR}/concurrentqueue/")

target_compile_features(concurrentqueue INTERFACE cxx_std_11)

