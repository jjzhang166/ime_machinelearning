cmake_minimum_required(VERSION 2.8)

project("IME Machine Learning")

# set Modules path to find dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# For YouCompleteMe integration
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Set C++11 as standard compilation
add_definitions("-std=c++11")

include_directories(.)

set(RUNTIME_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_DEBUG_POSTFIX -d)

add_subdirectory(utils)
add_subdirectory(neuralnetwork)
add_subdirectory(geneticalgorithm)

option(BUILD_DOCUMENTATION "ON to build documentation. OFF to ignore them." OFF)
if (BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif()

option(BUILD_EXAMPLES "ON to build examples. OFF to ignore them." ON)
if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

option(BUILD_CHALLENGES "ON to build challeges. OFF to ignore them." ON)
if (BUILD_CHALLENGES)
  add_subdirectory(challenges)
endif()

# Graphical Client
option(BUILD_CLIENT "ON to build graphical client. OFF to ignore them." ON)
if (BUILD_CLIENT)
  find_package(PkgConfig)
  pkg_search_module(GLFW glfw3 REQUIRED)
  find_package(Box2D REQUIRED)

  if (GLFW_FOUND AND BOX2D_FOUND)
    include_directories(${GLFW_INCLUDE_DIRS})
    include_directories(${BOX2D_INCLUDE_DIRS})
    add_subdirectory(client)
  else()
    message(SEND_ERROR "GLFW not found!")
    set(BUILD_CLIENT OFF)
  endif()
endif()
