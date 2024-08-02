#include "glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

const unsigned int SCR_WIDTH    = 1600;
const unsigned int SCR_HEIGHT   = 900;
unsigned int current_scr_width  = SCR_WIDTH;
unsigned int current_scr_height = SCR_HEIGHT;

glm::vec2 zoom   = glm::vec2(2.0f, 2.0f);
glm::vec2 offset = glm::vec2(0.75f, 0.5f);

int main(int, char**){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

    float box_vertices[] = {
        -0.1, -0.1, 0.0,    -1.0, -1.0,
         0.1, -0.1, 0.0,     1.0, -1.0,
        -0.1,  0.1, 0.0,    -1.0,  1.0,

         0.1, -0.1, 0.0,     1.0, -1.0,
         0.1,  0.1, 0.0,     1.0,  1.0,
        -0.1,  0.1, 0.0,    -1.0,  1.0
    };

    unsigned int VBO;
    unsigned int VAO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    Shader shader = Shader("/home/stevica/openGL_projects/pond/shaders/v_shader.glsl",
                           "/home/stevica/openGL_projects/pond/shaders/f_shader.glsl");

    shader.use();

    glm::vec2 c1 = glm::vec2(0.0f, 0.0f);

    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setFloat("screen_resolution", (float)current_scr_height/current_scr_width);
        shader.setVec2("center", 0.0f, 0.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader.setVec2("center", 0.4f, -0.2f);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    current_scr_height = height;
    current_scr_width  = width;
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



