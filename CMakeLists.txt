# Specify the version to use
cmake_minimum_required(VERSION 2.6)
project(n_graph_miner)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_executable(n_graph_miner ${SOURCES} ${HEADERS})