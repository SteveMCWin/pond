#include "app.h"

// GLFWwindow* app::init(){
App::App(){

    // GLFW window setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);

    Global::aspect_ratio = (float)mode->height / (float)mode->width;

    this->window = glfwCreateWindow(mode->width, mode->height, "Stevica :D", NULL, NULL);

    try{
        if(this->window == NULL){
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
    } catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    glfwMakeContextCurrent(window);

    try{
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }
    } catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xFFFF);

    glfwSetWindowAttrib(window, GLFW_FLOATING, GLFW_TRUE);
    glfwSwapInterval(0);

    glLineWidth(3.0f);

    this->renderer = new FishRenderer();

    this->handler.createFish();

    this->last_frame = 0.0f;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->window, true);

    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);

    glfwSwapBuffers(window);
    glfwPollEvents();

}

App::~App(){

}

void App::Run(){

    float current_frame = glfwGetTime();

    float delta_time = current_frame - this->last_frame;
    this->last_frame = current_frame;

    this->process_input();

    this->handler.resetBoxSizes();
    this->handler.boxTheFish();

    this->handler.handleFishMovement(delta_time);

    this->renderer->renderFish(this->handler.allFish, this->handler.number_of_fish);

    this->handle_imgui();

}

void App::handle_imgui(){

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

            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();   
        }

        // Check if mouse is interacting with ImGui windows
        bool mouseOverImGui = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered();

        // Enable/Disable mouse passthrough accordingly
        glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, mouseOverImGui ? GLFW_FALSE : GLFW_TRUE);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

bool App::isRunning(){
    return !glfwWindowShouldClose(this->window);
}

void App::process_input(){
    if(glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
