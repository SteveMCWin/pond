#include "app.h"
#include "imgui.h"
#include "serializer.h"
#include <GLFW/glfw3.h>

App::App(){

    Serializer::loadData();

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
    this->update_limit_framerate();

    glLineWidth(3.0f);

    this->renderer = new FishRenderer();

    this->handler = new FishHandler();
    this->handler->createFish();

    this->last_frame = 0.0f;
    this->o_first_touch = false;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->window, true);

    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);

}

App::~App(){

    if(Serializer::store_on_exit) Serializer::storeData();

    delete this->handler;
    delete this->renderer;

    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void App::Run(){

    float current_frame = glfwGetTime();

    float delta_time = current_frame - this->last_frame;
    this->last_frame = current_frame;

    this->process_input();

    this->handler->resetBoxSizes();
    this->handler->boxTheFish();

    this->handler->handleFishMovement(delta_time);

    this->renderer->renderFish(this->handler->allFish, this->handler->number_of_fish);

    this->handle_imgui();

    glfwSwapBuffers(window);
    glfwPollEvents();

}

void App::handle_imgui(){

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if(Serializer::show_gui)
    {
        ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        int num_of_fish = static_cast<int>(Serializer::number_of_fish);
        if(ImGui::InputInt("Number of fish", &num_of_fish)){
            Serializer::number_of_fish = std::min(std::max(1, num_of_fish), MAX_NUM_OF_FISH);
            this->handler->update_num_of_fish();
        }

        if(ImGui::ColorEdit3("Fish eye color", &Serializer::fish_eye_color[0])){
            this->renderer->update_fish_eye_color();
        }

        if(ImGui::ColorEdit3("Fish fin color", &Serializer::fish_fin_color[0])){
            this->renderer->update_fish_fin_color();
        }

        if(ImGui::ColorEdit3("Fish body color", &Serializer::fish_body_color[0])){
            this->renderer->update_fish_body_color();
        }

        if(ImGui::Checkbox("Use solid color", &Serializer::use_solid_color)){
            this->renderer->update_use_solid_color();
        }

        if(ImGui::InputFloat("Cohesion intensity", &Serializer::cohesion_intensity)){
            this->handler->update_cohesion_intensity();
        }

        if(ImGui::InputFloat("Alignment intensity", &Serializer::alignment_intensity)){
            this->handler->update_alignment_intensity();
        }

        if(ImGui::InputFloat("Separation intensity", &Serializer::separation_intensity)){
            this->handler->update_separation_intensity();
        }

        if(ImGui::InputFloat("Edge Evasion intensity", &Serializer::edge_evasion_intensity)){
            this->handler->update_edge_evasion_intensity();
        }

        if(ImGui::Checkbox("Limit framerate", &Serializer::is_framerate_limited)){
            this->update_limit_framerate();
        }

        ImGui::Checkbox("Store changes on exit", &Serializer::store_on_exit);

        if(ImGui::Button("Store changes")){
            Serializer::storeData();
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::End();   
    }

    // Check if mouse is interacting with ImGui windows
    bool mouseOverImGui = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup);
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
    if(glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS){
        if(!o_first_touch){
            o_first_touch = true;
            Serializer::show_gui = !Serializer::show_gui;
        }
    }
    if(glfwGetKey(this->window, GLFW_KEY_O) == GLFW_RELEASE){
        o_first_touch = false;
    }
}

void App::update_limit_framerate(){
    glfwSwapInterval(Serializer::is_framerate_limited ? 1 : 0);
}
