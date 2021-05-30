cmake_minimum_required(VERSION 3.5)
project(goosestd LANGUAGES CXX)

add_library(goosestd INTERFACE)
set (CMAKE_CXX_STANDARD 17)

target_include_directories(goosestd INTERFACE include)