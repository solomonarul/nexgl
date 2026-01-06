cmake_minimum_required(VERSION 3.10)

project(glad_gles2)

add_library(
    ${PROJECT_NAME}
    STATIC
        "src/glad.c"
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        "inc"
    PRIVATE
        "inc/glad"
)