cmake_minimum_required(VERSION 3.15)

project(logger
	VERSION 0.1
	LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(dlog
	src/dlog.c)

target_include_directories(dlog
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(dlog
	PRIVATE
		-Wall -Wextra)

add_executable(dlog_demo
	demo/main.c)

target_link_libraries(dlog_demo
	PRIVATE
		dlog)

target_compile_options(dlog_demo
	PRIVATE
		-Wall -Wextra)

enable_testing()
