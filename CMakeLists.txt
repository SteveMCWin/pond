cmake_minimum_required(VERSION 3.0.0)

project(my_opengl_project VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

find_package(glfw3 3.4 QUIET)
if(NOT GLFW_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(GLFW REQUIRED glfw3)
    else()
        message(FATAL_ERROR "GLFW not found! Install it via package manager or vcpkg.")
    endif()
endif()
# find_package(glm REQUIRED)
find_path(GLM_INCLUDE_DIR glm/glm.hpp)
if(NOT GLM_INCLUDE_DIR)
    message(FATAL_ELRROR "GLM not found. Install it via package manager or vcpkg.")
endif()

include(FetchContent)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

include_directories(-I/usr/include/libpng16 ${GLM_INCLUDE_DIR})

add_executable(my_opengl_project 
    main.cpp
    # glad.c
    fish.cpp
    fishRenderer.cpp
    bezier_filled.cpp
    global.cpp
    fishHandler.cpp
    texture.cpp
    stb_image.cpp)

target_link_libraries(my_opengl_project glfw OpenGL::GL glad)
