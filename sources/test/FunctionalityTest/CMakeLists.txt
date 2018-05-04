cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpic++")

file(GLOB_RECURSE core_src "../../core/*.cc")
file(GLOB_RECURSE generators_src "../../generators/*.cc")
file(GLOB_RECURSE parser_src "../../parser/src/*.cpp")
file(GLOB_RECURSE test_src "./*.cc")
file(GLOB_RECURSE cluster_src "../../projects/MpiCluster/src/*.cc")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_CXX_FLAGS "-O3 -g -Wall -std=c++11 -lboost_mpi-mt -lboost_serialization -lboost_system -lboost_filesystem")

include_directories(
${CMAKE_SOURCE_DIR}/sources/projects/MpiCluster/include
${CMAKE_SOURCE_DIR}/sources/core/StructuralElements/include
${CMAKE_SOURCE_DIR}/sources/core/DynamicalEquation/include
${CMAKE_SOURCE_DIR}/sources/core/UpwardInfluence/include
${CMAKE_SOURCE_DIR}/sources/core/DownwardInfluence/include
${CMAKE_SOURCE_DIR}/sources/core/VectorFieldReconfiguration/include
${CMAKE_SOURCE_DIR}/sources/core/Utility/general/include
${CMAKE_SOURCE_DIR}/sources/core/Utility/class/include
${CMAKE_SOURCE_DIR}/sources/generators/StructureGenerators/include
${CMAKE_SOURCE_DIR}/sources/generators/DynamicsGenerators/include
${CMAKE_SOURCE_DIR}/sources/generators/InitialConditionGenerators/include
${CMAKE_SOURCE_DIR}/sources/generators/GeneralGenerators/include
${CMAKE_SOURCE_DIR}/sources/parser/include
${CMAKE_SOURCE_DIR}/sources/external
${CMAKE_SOURCE_DIR}/sources/projects/MainProject/include
${CMAKE_SOURCE_DIR}/sources/projects/HebbianProject/include)

add_executable(testGeneticAlgorithm
${CMAKE_SOURCE_DIR}/sources/test/FunctionalityTest/TestGeneticAlgorithm.cc
${test_src} ${cluster_src} ${core_src} ${generators_src} ${parser_src})
