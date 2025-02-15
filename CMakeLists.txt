cmake_minimum_required(VERSION 3.23)

project(uvlib)

option(ENABLE_TESTS "Enable test build" ON)

set(CMAKE_CXX_STANDARD 20)

if (ENABLE_TESTS)
	# Add Google Test as a submodule
	set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
	set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
	set(GOOGLETEST_PATH ${CMAKE_SOURCE_DIR}/external/googletest)

	add_subdirectory(${GOOGLETEST_PATH} ${CMAKE_BINARY_DIR}/googletest-build)

	file(GLOB_RECURSE LIBLVGL_HEADERS 
		${INCLUDE_DIR}/liblvgl/*.h 
		${INCLUDE_DIR}/liblvgl/*.hpp 
	)

	file(GLOB_RECURSE PROS_HEADERS 
		${INCLUDE_DIR}/pros/*.h 
		${INCLUDE_DIR}/pros/*.hpp 
	)

	# Add the test executable
	add_executable(
		uvlib-tests

		${LIBLVGL_HEADERS}
		${PROS_HEADERS}

		# include/main.h
		# include/api.h
		
		src/uvlib/scheduler.cpp
		src/uvlib/subsystem.cpp
		src/uvlib/commands/command.cpp
		src/uvlib/commands/commandptr.cpp
		src/uvlib/input/controller.cpp
		src/uvlib/input/trigger.cpp
		src/uvlib/input/joystick.cpp

		src/main.cpp
		src/test.cpp
	)

	target_include_directories(
		uvlib-tests
		PUBLIC

		${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/pros
		${CMAKE_SOURCE_DIR}/liblvgl
		${CMAKE_SOURCE_DIR}/tests
	)

	target_compile_definitions(uvlib-tests PRIVATE TESTING)

	# Link Google Test
	target_link_libraries(
		uvlib-tests

		gtest
		gtest_main

		${FIRMWARE_DIR}/libc.a
    ${FIRMWARE_DIR}/liblvgl.a
    ${FIRMWARE_DIR}/libm.a
    ${FIRMWARE_DIR}/libpros.a
	)

	enable_testing()
	add_test(NAME uvlib-tests COMMAND uvlib-tests)
else()
	# Add the executables
	add_executable(
		uvlib
		src/main.cpp
	)

	target_include_directories(
		uvlib
		PUBLIC
		${CMAKE_SOURCE_DIR}/include
	)
endif()