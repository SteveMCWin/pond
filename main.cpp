#include "glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "global.h"
#include "shader.h"
#include "fish.h"
#include "fishRenderer.h"
#include "fishHandler.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

const unsigned int SCR_WIDTH    = 1600;
const unsigned int SCR_HEIGHT   = 900;
const float ASPECT_RATIO      = (float)SCR_HEIGHT/SCR_WIDTH;

glm::vec2 offset = glm::vec2(0.75f, 0.5f);

float delta_time = 0.0f;
float last_frame = 0.0f;

int main(int, char**){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    // glfwSwapInterval(0);
    // vblank_mode=0 ./my_opengl_project 

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Stevica :D", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

    Shader circleShader = Shader("/home/stevica/openGL_projects/pond/shaders/v_shader.glsl",
                           "/home/stevica/openGL_projects/pond/shaders/f_shader.glsl");
    Shader outlineShader = Shader("/home/stevica/openGL_projects/pond/shaders/v_solid.glsl",
                                  "/home/stevica/openGL_projects/pond/shaders/f_solid.glsl");
    Shader bezier_shader = Shader("/home/stevica/openGL_projects/pond/shaders/v_bezier.glsl",
                                  "/home/stevica/openGL_projects/pond/shaders/f_bezier.glsl");

    circleShader.use();

    float distances[] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    float radii[]     = {1.2f, 1.3f, 1.4f, 1.4f, 1.3f, 1.2f, 1.0f, 0.8f, 0.6f, 0.5f, 0.3f, 0.2f};
    glm::vec2 centers[] = {
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

    // std::cout << "Size of distances " << sizeof(distances)/sizeof(float) << std::endl;
    // std::cout << "Size of radii " << sizeof(radii)/sizeof(float) << std::endl;
    // std::cout << "Size of centers " << sizeof(centers)/sizeof(glm::vec2) << std::endl;

    FishRenderer* renderer = new FishRenderer();

    FishHandler fishHandler;

    // fishHandler.addFish(12, centers, distances, radii, 12);
    // Fish fishy = Fish(12, centers, distances, radii, 7);
    // fishHandler.addFish(fishy);

    // centers[0] = glm::vec2(-20.0f, -12.0f);
    // fishHandler.addFish(12, centers, distances, radii, 12);

    for(int i = 0; i < 10; i++){
        centers[0] = glm::vec2((Global::GetRandomFloat()*2.0f - 1.0f), Global::GetRandomFloat()*2.0f - 1.0f) * Global::bottomLeftCorner;
        fishHandler.addFish(12, centers, distances, radii, i, 12);
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int frameCounter = 60;

    while(!glfwWindowShouldClose(window)){

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        if(frameCounter > 50){
            // std::cout << "\rFPS: " << 1.0f/delta_time << std::flush;
            frameCounter = -1;
        }
        frameCounter++;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // glm::vec2 move_point = glm::vec2(xpos/SCR_WIDTH, 1.0f - ypos/SCR_HEIGHT) * glm::vec2(Global::screenHalfSize * 2) - glm::vec2(Global::screenHalfSize);
        // move_point.y *= ASPECT_RATIO;

        // for(int i = 0; i < f.numOfJoints; i++){
        //     std::cout << "pos[" << 2*i   << "]: <" << f.outline_vertices[4*i]   << ", " << f.outline_vertices[4*i+1] << ">" << std::endl;
        //     std::cout << "pos[" << 2*i+1 << "]: <" << f.outline_vertices[4*i+2] << ", " << f.outline_vertices[4*i+3] << ">" << std::endl;
        // }
        
        for(Fish& f : fishHandler.allFish){

            // if(glm::length(move_point - f.joints[0].Center) > 0.1)
                // f.Move(move_point - f.joints[0].Center);

            // f.Move(f.hit_checks[f.hit_checks.size()/2 + f.hit_checks_result]);
            glm::vec2 newMoveDir = f.joints[0].moveDirection;
            newMoveDir += fishHandler.calcFishMoveDir(f) * delta_time;
            f.Move(newMoveDir, delta_time);

            renderer->renderFishSideFins(f, glm::vec2(1.5f, 0.5f), glm::vec2(1.0f, 0.3f), circleShader);
            renderer->renderFishBody(f, circleShader, outlineShader);
            renderer->renderFishEyes(f, glm::vec2(0.15f, 0.4f), circleShader);
            renderer->renderFishTailFin(f, bezier_shader);
            renderer->renderFishBackFin(f, bezier_shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    std::cout << std::endl;

    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}



