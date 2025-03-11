#ifndef APP
#define APP

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>

#include "fishHandler.h"
#include "fishRenderer.h"
#include "global.h"
#include "serializer.cpp"

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class App{

public:
     App();
    ~App();
    void Run();
    bool isRunning();

private:

    GLFWwindow* window;
    FishRenderer* renderer;
    FishHandler handler;

    float last_frame;

    void process_input();
    void handle_imgui();
};


#endif // !APP
