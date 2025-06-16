#ifndef FISH_SERIALIZER
#define FISH_SERIALIZER

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>

#include "global.h"

class Serializer {

public:
    static void loadData();
    static void storeData();

    static glm::vec3 fish_eye_color;
    static glm::vec3 fish_fin_color;
    static glm::vec3 fish_body_color;
    static std::string fish_tex_path;
    static unsigned int number_of_fish;
    static float cohesion_intensity;
    static float alignment_intensity;
    static float separation_intensity;
    static float edge_evasion_intensity;
    static float pixelation_amount;
    static bool is_framerate_limited;
    static bool use_solid_color;
    static bool use_pixelation;
    static bool store_on_exit;
    static bool show_gui;

};

#endif // !FISH_SERIALIZER
