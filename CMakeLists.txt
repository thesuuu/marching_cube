cmake_minimum_required(VERSION 3.0)

project(Marching_Cube)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
	src/generator.cpp
	src/main_marching_cube.cpp
	src/marching_cube.cpp
	src/utilities.cpp)

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})