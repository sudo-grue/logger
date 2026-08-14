include(FetchContent)
FetchContent_Declare(unity
	GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
	GIT_TAG v2.7.0)

FetchContent_GetProperties(unity)
if(NOT unity_POPULATED)
	FetchContent_Populate(unity)
endif()

# See tpool's tests/CMakeLists.txt for why we build our own minimal target
# instead of add_subdirectory()-ing Unity's own CMakeLists.txt.
add_library(unity STATIC
	${unity_SOURCE_DIR}/src/unity.c)

target_include_directories(unity
	PUBLIC
		${unity_SOURCE_DIR}/src)

add_executable(test_dlog
	test_dlog.c)

target_link_libraries(test_dlog
	PRIVATE
		dlog
		unity)

target_compile_options(test_dlog
	PRIVATE
		${LOGGER_WARNING_FLAGS})

add_test(NAME dlog COMMAND test_dlog)
