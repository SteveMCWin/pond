#include "glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "shader.h"
#include "fish.h"
#include "fishRenderer.h"

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

glm::vec2 zoom   = glm::vec2(2.0f, 2.0f);
glm::vec2 offset = glm::vec2(0.75f, 0.5f);

int main(int, char**){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

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

    Shader shader = Shader("/home/stevica/openGL_projects/pond/shaders/v_shader.glsl",
                           "/home/stevica/openGL_projects/pond/shaders/f_shader.glsl");

    shader.use();

    float distances[] = {1.0f, 2.2f, 1.5f, 2.0f, 1.6f};
    float radii[]     = {1.2f, 1.5f, 1.0f, 0.8f, 0.5f};
    glm::vec2 centers[] = {glm::vec2(1.0f, 1.0f), glm::vec2(-0.1f, -0.1f), glm::vec2(-0.1f, -1.5f), glm::vec2(-0.1f, -1.7f), glm::vec2(-1.0f, -1.9f)};

    Fish f = Fish(5, centers, distances, radii);
    FishRenderer renderer;

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec2 move_point = glm::vec2(xpos/SCR_WIDTH, 1.0f - ypos/SCR_HEIGHT) * glm::vec2(40.0) - glm::vec2(20.0f, 20.0f);
        move_point.y *= ASPECT_RATIO;

        if(glm::length(move_point - f.joints[0].Center) > 0.1){
            f.Move(move_point - f.joints[0].Center);
        }

        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(f.joints[0].Center + f.joints[0].moveDirection, 0.0f));
        // I wanted to experiment by drawing the tip of the nose of the fish but I realized it's too much for tonight
        // The idead I got for connecting the dots is basically just making a vertices array in each fish that will change each MoveFish call and which will be used by the
        // fish renderer
        // the problem is that I have to figure out what data do i write into the buffer so I'll need to experiment
        // (by 'what data' I mean if it should range from -1 to 1, which it definitely shouldn't (I think???))
    
        renderer.renderFish(f, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

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

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE)){
        zoom.x += 0.01 * zoom.x;
        zoom.y += 0.01 * zoom.y;
    }
    else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        zoom.x -= 0.01 * zoom.x;
        zoom.y -= 0.01 * zoom.y;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        offset.x += 0.001 * offset.x * offset.x / 1.5;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        offset.x -= 0.001 * offset.x * offset.x / 1.5;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        offset.y -= 0.001 * offset.y * offset.y;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        offset.y += 0.001 * offset.y * offset.y;
    }

}



