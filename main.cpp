#include <cmath>
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "global.h"
#include "shader.h"
#include "compute_shader.h"
#include "fish.h"
#include "fishRenderer.h"
#include "fishHandler.h"
#include "texture.h"

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

// const unsigned int WINDOW_WIDTH    = 1600;
// const unsigned int WINDOW_HEIGHT   =  900;

float delta_time = 0.0f;
float last_frame = 0.0f;

int main(int, char**){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    // glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Stevica :D", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetWindowAttrib(window, GLFW_FLOATING, GLFW_TRUE);
    glfwSwapInterval(0);

    glLineWidth(3.0f);

    // glEnable(GL_MULTISAMPLE);

    Shader circleShader = Shader("/home/stevica/openGL_projects/pond/shaders/v_shader.glsl",
                           "/home/stevica/openGL_projects/pond/shaders/f_shader.glsl");
    Shader outlineShader = Shader("/home/stevica/openGL_projects/pond/shaders/v_solid.glsl",
                                  "/home/stevica/openGL_projects/pond/shaders/f_solid.glsl");
    Shader bezierShader = Shader("/home/stevica/openGL_projects/pond/shaders/v_bezier.glsl",
                                  "/home/stevica/openGL_projects/pond/shaders/f_bezier.glsl");
    Shader screenShader = Shader("/home/stevica/openGL_projects/pond/shaders/v_screen.glsl",
                                 "/home/stevica/openGL_projects/pond/shaders/f_screen.glsl");
    Shader backgroundShader = Shader("/home/stevica/openGL_projects/pond/shaders/v_background.glsl",
                                     "/home/stevica/openGL_projects/pond/shaders/f_background.glsl");

    float distances[Global::numberOfJoints]   = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};   // desired distances from one circle center to another
    float radii[Global::numberOfJoints]       = {1.2f, 1.3f, 1.4f, 1.4f, 1.3f, 1.2f, 1.0f, 0.8f, 0.6f, 0.5f, 0.3f, 0.2f};   // radii of each visible circle
    glm::vec2 centers[Global::numberOfJoints] = {     // position of each circle center, the values below are pretty much random, wouldn't make a big difference if they were all (0, 0)
        glm::vec2(1.0f, 1.0f),
        glm::vec2(-0.1f, -0.1f),
        glm::vec2(-0.1f, -1.5f),
        glm::vec2(-0.1f, -1.7f),
        glm::vec2(-1.0f, -1.9f),
        glm::vec2(-1.0f, -1.8f),
        glm::vec2(-1.0f, -1.7f),
        glm::vec2(-1.0f, -1.6f),
        glm::vec2(-1.1f, -1.9f),
        glm::vec2(-1.2f, -1.9f),
        glm::vec2(-1.3f, -1.9f),
        glm::vec2(-1.4f, -1.9f),
    };

    FishRenderer* renderer = new FishRenderer();

    FishHandler fishHandler;

    for(int i = 0; i < Global::numberOfFish; i++){
        centers[0] = glm::vec2((Global::GetRandomFloat()*2.0f - 1.0f), Global::GetRandomFloat()*2.0f - 1.0f) * Global::bottomLeftCorner;
        fishHandler.addFish(centers, distances, radii, i);
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);

    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);

    

    int frameCounter = 0;
    double fps_timer = 0;

    while(!glfwWindowShouldClose(window)){


        float current_frame = glfwGetTime();

        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        processInput(window);

        screenShader.use();
        screenShader.setFloat("iTime", current_frame);

        // fishHandler.calcFishHitChecks();

        // FrameMarkStart("Boxing");

        fishHandler.boxTheFish();
        
        for(Fish& f : fishHandler.allFish){
            glm::vec2 newMoveDir = fishHandler.calcFishMoveDir(f, delta_time);
            f.updateMoveDir(newMoveDir);
        }

        for(Fish& f : fishHandler.allFish){
            f.Move(delta_time);
        }

        fishHandler.resetBoxSizes();

        // FrameMarkEnd("Boxing");

        renderer->renderFish(fishHandler.allFish, circleShader, outlineShader, bezierShader, screenShader,
                             backgroundShader, glm::vec2(1.5f, 0.5f), glm::vec2(1.0f, 0.3f), glm::vec2(0.15f, 0.4f));

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();   
        }

        // Check if mouse is interacting with ImGui windows
        bool mouseOverImGui = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered();

        // Enable/Disable mouse passthrough accordingly
        glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, mouseOverImGui ? GLFW_FALSE : GLFW_TRUE);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}



// void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // glViewport(0, 0, width, height);
// }

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}



