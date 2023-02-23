cmake_minimum_required(VERSION 3.21)

project(stb)

message("-- ${PROJECT_NAME} : static")

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

target_sources(
    ${PROJECT_NAME}
    PRIVATE ${SRC_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
