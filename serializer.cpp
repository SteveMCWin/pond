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

namespace Serializer {

glm::vec3 fin_color;
std::string texture_path;
unsigned int number_of_fish;
float cohesion_intensity;
float alignment_intensity;
float separation_intensity;
float edge_evasion_intensity;
bool limit_framerate;
bool use_texture;

void loadData(){
    std::ifstream file(DATA_PATH);
    if (!file) {
        std::cout << "No config file found, using default values.\n";

        Serializer::fin_color = DEFAULT_FIN_COLOR;
        Serializer::texture_path = (std::filesystem::path)TEXTURES_PATH / "koi.jpg";
        Serializer::number_of_fish = DEFAULT_NUM_OF_FISH;
        Serializer::cohesion_intensity = DEFAULT_COHESION_INTENSITY;
        Serializer::alignment_intensity = DEFAULT_ALIGNMENT_INTENSITY;
        Serializer::separation_intensity = DEFAULT_SEPARATION_INTENSITY;
        Serializer::edge_evasion_intensity = DEFAULT_EDGE_EVASION_INTENSITY;
        Serializer::limit_framerate = true;
        Serializer::use_texture = true;
        return;
    }

    file >> fin_color.r >> fin_color.g >> fin_color.b;
    file >> texture_path;
    file >> limit_framerate >> number_of_fish;
    file >> cohesion_intensity >> alignment_intensity >> separation_intensity >> edge_evasion_intensity;
    file >> use_texture;

    file.close();

    Serializer::number_of_fish = std::min(Serializer::number_of_fish, (unsigned int)MAX_NUM_OF_FISH);
}

void storeData(){
    std::ofstream file(DATA_PATH);
    if (!file) {
        std::cerr << "Error writing config file!\n";
        return;
    }

    file << fin_color.r << " " << fin_color.g << " " << fin_color.b << "\n";
    file << texture_path << "\n";
    file << limit_framerate << " " << number_of_fish << "\n";
    file << cohesion_intensity << " " << alignment_intensity << " " << separation_intensity << " " << edge_evasion_intensity << "\n";
    file << use_texture << "\n";

    file.close();
}


}

#endif // !FISH_SERIALIZER
