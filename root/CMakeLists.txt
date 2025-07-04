# TODO 1: Set the minimum required version of CMake to be 3.10

cmake_minimum_required(VERSION 3.10)

project(application VERSION 1.0 LANGUAGES CXX)

add_subdirectory(code/modules/approximator)  # Your C module
add_subdirectory(code/application)