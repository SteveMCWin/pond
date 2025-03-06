#ifndef SETUP
#define SETUP

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "fishRenderer.h"
#include "global.h"

namespace setup {

    GLFWwindow* init();

}


#endif // !SETUP
