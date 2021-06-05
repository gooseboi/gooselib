cmake_minimum_required(VERSION 3.5)
project(gooselib LANGUAGES CXX)

add_library(gooselib INTERFACE)
set (CMAKE_CXX_STANDARD 17)

target_include_directories(gooselib INTERFACE include)
