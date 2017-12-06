cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(MultilayerModel)

file(GLOB_RECURSE core_src "sources/core/*.cc")
file(GLOB_RECURSE generators_src "sources/generators/src/*.cc")
file(GLOB_RECURSE parser_src "sources/parser/src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -lboost_unit_test_framework")

include_directories(
${CMAKE_SOURCE_DIR}/sources/core/StructuralElements/include
${CMAKE_SOURCE_DIR}/sources/core/DynamicalEquation/include
${CMAKE_SOURCE_DIR}/sources/core/UpwardInfluence/include
${CMAKE_SOURCE_DIR}/sources/core/DownwardInfluence/include
${CMAKE_SOURCE_DIR}/sources/core/VectorFieldReconfiguration/include
${CMAKE_SOURCE_DIR}/sources/core/Utility/include
${CMAKE_SOURCE_DIR}/sources/generators/include
${CMAKE_SOURCE_DIR}/sources/parser/include
${CMAKE_SOURCE_DIR}/sources/external)

add_executable(test ${CMAKE_SOURCE_DIR}/sources/test/test_StructureGeneration.cc ${core_src} ${generators_src} ${parser_src})
add_executable(generate ${CMAKE_SOURCE_DIR}/sources/runners/GenerateMain.cc ${core_src} ${generators_src} ${parser_src})
add_executable(simulate ${CMAKE_SOURCE_DIR}/sources/runners/SimulateMain.cc ${core_src} ${generators_src} ${parser_src})
