cmake_minimum_required(VERSION 3.0.0)

# add_subdirectory("assimp")

project(my_opengl_project VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

# find_package(glm CONFIG REQUIRED)
# target_link_libraries(main PRIVATE glm::glm-header-only)

add_executable(my_opengl_project 
    main.cpp
    glad.c)

include_directories(/urs/include/freetype2 -I/usr/local/include/freetype2 -I/usr/include/libpng16)

target_link_libraries(my_opengl_project glfw OpenGL::GL)
