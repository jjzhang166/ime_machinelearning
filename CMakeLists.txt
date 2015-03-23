cmake_minimum_required(VERSION 2.8)

project("IME Machine Learning")

# For YouCompleteMe integration
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Set C++11 as standard compilation
#add_definitions("-str=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(RUNTIME_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(neuralnet)

option(BUILD_EXAMPLES "ON to build examples. OFF to ignore them." ON)
if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
