cmake_minimum_required(VERSION 3.0.0)

project(my_opengl_project VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.4 REQUIRED)

add_executable(my_opengl_project 
    main.cpp
    glad.c
    fish.cpp
    fishRenderer.cpp
    bezier_filled.cpp
    global.cpp
    fishHandler.cpp
    texture.cpp
    stb_image.cpp)

include_directories(-I/usr/include/libpng16)

target_link_libraries(my_opengl_project glfw OpenGL::GL)
