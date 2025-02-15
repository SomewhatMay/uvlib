cmake_minimum_required(VERSION 3.3)

set(This uvlib)
set(ThisTest ${This}-test)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

option(ENABLE_TESTS "Enable test build" ON)

## Build Sources ##

file(GLOB_RECURSE UVLIB_SOURCES "src/uvlib/*.c" "src/uvlib/*.cpp")
file(GLOB_RECURSE PROS_SOURCES "src/pros/*.c" "src/pros/*.cpp")
file(GLOB_RECURSE LIBLVGL_SOURCES "src/liblvgl/*.c" "src/liblvgl/*.cpp")

set(Sources
  ${UVLIB_SOURCES}
  ${PROS_SOURCES}
  ${LIBLVGL_SOURCES}
)

file(GLOB_RECURSE UVLIB_HEADERS "include/uvlib/*.c" "include/uvlib/*.cpp")
file(GLOB_RECURSE PROS_HEADERS "include/pros/*.c" "include/pros/*.cpp")
file(GLOB_RECURSE LIBLVGL_HEADERS "include/liblvgl/*.c" "include/liblvgl/*.cpp")

set(Headers
  include/main.h
  include/api.h
  ${UVLIB_SOURCES}
  ${PROS_SOURCES}
  ${LIBLVGL_SOURCES}
)

## Main ##

if (ENABLE_TESTS)
	# Add Google Test as a submodule
	set(GOOGLETEST_PATH ${CMAKE_SOURCE_DIR}/external/googletest)
	add_subdirectory(${GOOGLETEST_PATH} ${CMAKE_BINARY_DIR}/googletest-build)

  file(GLOB_RECURSE TEST_SOURCES "tests/*.c" "tests/*.cpp")

	# Add the test executable
	add_executable(
    tests/main.cpp
    ${ThisTest}
		${Sources}
    ${Headers}
    ${TEST_SOURCES}
	)

	target_include_directories(
		${ThisTest}
		PUBLIC
		${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/tests
	)

	target_compile_definitions(${ThisTest} PRIVATE TESTING)

	# Link Google Test
	target_link_libraries(
		${ThisTest}
		gtest
		gtest_main
	)

	enable_testing()
	add_test(NAME ${ThisTest} COMMAND ${ThisTest})
else()
	# Add the executables
	add_executable(
    src/main.cpp
		${This}
		${Sources}
	)

	target_include_directories(
		${This}
		PUBLIC
		${CMAKE_SOURCE_DIR}/include
	)
endif()