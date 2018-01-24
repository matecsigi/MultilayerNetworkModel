cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpic++")

#file(GLOB_RECURSE cluster_src "sources/projects/MpiCluster/src/*.cc")
file(GLOB_RECURSE core_src "sources/core/*.cc")
file(GLOB_RECURSE generators_src "sources/generators/*.cc")
file(GLOB_RECURSE parser_src "sources/parser/src/*.cpp")
#file(GLOB_RECURSE project_barabasi_src "sources/projects/BarabasiMultilayer/src/*.cc")
#file(GLOB_RECURSE project_hebbian_src "sources/projects/HebbianProject/src/*.cc")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_CXX_FLAGS "-O3 -g -Wall -std=c++11 -lboost_mpi")

include_directories(
${CMAKE_SOURCE_DIR}/sources/projects/MpiCluster/include
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
${CMAKE_SOURCE_DIR}/sources/projects/HebbianProject/include)

add_executable(cluster
${CMAKE_SOURCE_DIR}/sources/projects/MpiCluster/ClusterMain.cc ${core_src}
${generators_src} ${parser_src})

