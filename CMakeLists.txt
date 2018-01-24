cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(MultilayerModel)

add_subdirectory(sources/projects/MpiCluster)

file(GLOB_RECURSE core_src "sources/core/*.cc")
file(GLOB_RECURSE generators_src "sources/generators/*.cc")
file(GLOB_RECURSE parser_src "sources/parser/src/*.cpp")
file(GLOB_RECURSE project_barabasi_src "sources/projects/BarabasiMultilayer/src/*.cc")
file(GLOB_RECURSE project_hebbian_src "sources/projects/HebbianProject/src/*.cc")
file(GLOB_RECURSE cluster_src "sources/projects/MpiCluster/src/*.cc")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_CXX_FLAGS "-O3 -g -Wall -std=c++11 -lboost_unit_test_framework")

include_directories(
${CMAKE_SOURCE_DIR}/sources/core/StructuralElements/include
${CMAKE_SOURCE_DIR}/sources/core/DynamicalEquation/include
${CMAKE_SOURCE_DIR}/sources/core/UpwardInfluence/include
${CMAKE_SOURCE_DIR}/sources/core/DownwardInfluence/include
${CMAKE_SOURCE_DIR}/sources/core/VectorFieldReconfiguration/include
${CMAKE_SOURCE_DIR}/sources/core/Utility/include
${CMAKE_SOURCE_DIR}/sources/generators/StructureGenerators/include
${CMAKE_SOURCE_DIR}/sources/generators/DynamicsGenerators/include
${CMAKE_SOURCE_DIR}/sources/generators/InitialConditionGenerators/include
${CMAKE_SOURCE_DIR}/sources/generators/GeneralGenerators/include
${CMAKE_SOURCE_DIR}/sources/parser/include
${CMAKE_SOURCE_DIR}/sources/external
${CMAKE_SOURCE_DIR}/sources/projects/MainProject/include
${CMAKE_SOURCE_DIR}/sources/projects/BarabasiMultilayer/include
${CMAKE_SOURCE_DIR}/sources/projects/HebbianProject/include
${CMAKE_SOURCE_DIR}/sources/projects/MpiCluster/include)

add_executable(testing
${CMAKE_SOURCE_DIR}/sources/test/test_StructureGeneration.cc ${core_src}
${generators_src} ${parser_src} ${cluster_src})
add_executable(generate
${CMAKE_SOURCE_DIR}/sources/projects/MainProject/GenerateMain.cc ${core_src}
${generators_src} ${parser_src} ${cluster_src})
add_executable(simulate
${CMAKE_SOURCE_DIR}/sources/projects/MainProject/SimulateMain.cc ${core_src}
${generators_src} ${parser_src} ${cluster_src})
add_executable(simulateBarabasi
${CMAKE_SOURCE_DIR}/sources/projects/BarabasiMultilayer/SimulateBarabasiMain.cc
${core_src} ${generators_src} ${parser_src} ${project_barabasi_src} ${cluster_src})
add_executable(generateBarabasi
${CMAKE_SOURCE_DIR}/sources/projects/BarabasiMultilayer/GenerateBarabasiMain.cc
${core_src} ${generators_src} ${parser_src} ${project_barabasi_src} ${cluster_src})
add_executable(simulateHebbian
${CMAKE_SOURCE_DIR}/sources/projects/HebbianProject/simulateHebbian.cc
${core_src} ${generators_src} ${parser_src} ${project_barabasi_src}
${project_hebbian_src} ${cluster_src})
