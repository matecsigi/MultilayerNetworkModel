cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(MultilayerModel)

file(GLOB_RECURSE core_src "src/core/src/*.cc")
file(GLOB_RECURSE generators_src "src/generators/src/*.cc")
file(GLOB_RECURSE parser_src "src/parser/src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -lboost_unit_test_framework")

include_directories(${CMAKE_SOURCE_DIR}/src/core/include
${CMAKE_SOURCE_DIR}/src/generators/include
${CMAKE_SOURCE_DIR}/src/parser/include ${CMAKE_SOURCE_DIR}/src/external)

add_executable(test ${CMAKE_SOURCE_DIR}/src/test/test_StructureGeneration.cc ${core_src} ${generators_src} ${parser_src})
add_executable(generate ${CMAKE_SOURCE_DIR}/src/runners/GenerateMain.cc ${core_src} ${generators_src} ${parser_src})
add_executable(simulate ${CMAKE_SOURCE_DIR}/src/runners/SimulateMain.cc ${core_src} ${generators_src} ${parser_src})
