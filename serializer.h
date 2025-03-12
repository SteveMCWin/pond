#ifndef FISH_SERIALIZER
#define FISH_SERIALIZER

#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
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

    static glm::vec3 fin_color;
    static glm::vec3 body_color;
    static std::string custom_tex_path;
    static unsigned int number_of_fish;
    static float cohesion_intensity;
    static float alignment_intensity;
    static float separation_intensity;
    static float edge_evasion_intensity;
    static bool is_framerate_limited;
    static bool use_texture;

};

#endif // !FISH_SERIALIZER
