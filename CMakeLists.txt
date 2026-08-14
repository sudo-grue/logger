cmake_minimum_required(VERSION 3.15)

project(logger
	VERSION 0.1
	LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LOGGER_WARNING_FLAGS
	-Wall -Wextra -Wpedantic -Wvla -Wwrite-strings
	-Waggregate-return -Wstack-usage=512 -Werror)

add_library(dlog
	src/dlog.c)

target_include_directories(dlog
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(dlog
	PRIVATE
		${LOGGER_WARNING_FLAGS})

add_executable(dlog_demo
	demo/main.c)

target_link_libraries(dlog_demo
	PRIVATE
		dlog)

target_compile_options(dlog_demo
	PRIVATE
		${LOGGER_WARNING_FLAGS})

# Only expose dev-only targets (tests, tidy) when this is the top-level
# build, not when logger is pulled in as someone else's dependency (e.g.
# tpool's FetchContent). Otherwise logger's test suite would silently
# become part of a consumer's ctest run.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	enable_testing()
	add_subdirectory(tests)

	find_program(CLANG_TIDY_EXECUTABLE clang-tidy)
	if(CLANG_TIDY_EXECUTABLE)
		add_custom_target(tidy
			COMMAND ${CLANG_TIDY_EXECUTABLE}
				-checks=cert*,bugprone*,misc*,readability*,-cert-err33-c
				${CMAKE_CURRENT_SOURCE_DIR}/src/dlog.c
				--
				-std=c99
				-I${CMAKE_CURRENT_SOURCE_DIR}/src
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT "Running clang-tidy on logger sources")
	else()
		message(STATUS "clang-tidy not found: skipping tidy target")
	endif()
endif()
