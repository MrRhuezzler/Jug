cmake_minimum_required(VERSION 3.21)

project(libs)

add_subdirectory(imgui)

add_subdirectory(stb)

option(BUILD_STATIC_LIBS "Build Static Library" ON)
option(BUILD_SHARED_LIBS "Build Shared Library" OFF)
add_subdirectory(glm)
