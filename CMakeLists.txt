cmake_minimum_required(VERSION 3.12)

project(jug VERSION 0.0.1)

add_subdirectory(libs)

message("-- ${PROJECT_NAME} : static : ${PROJECT_VERSION}")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW REQUIRED glfw3)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

target_sources(
    ${PROJECT_NAME}
    PRIVATE ${SRC_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${GLEW_INCLUDE_DIRS}
    PRIVATE ${GLFW_INCLUDE_DIRS}
    PRIVATE ${OPENGL_INCLUDE_DIRS}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    imgui
    glm_static
    stb
)

add_subdirectory(examples)