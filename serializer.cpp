#include "serializer.h"

glm::vec3 Serializer::fish_eye_color = DEFAULT_EYE_COLOR;
glm::vec3 Serializer::fish_fin_color = DEFAULT_FIN_COLOR;
glm::vec3 Serializer::fish_body_color = DEFAULT_BODY_COLOR;
std::string Serializer::fish_tex_path = (std::filesystem::path)TEXTURES_PATH / "koi.jpg"; // CHANGE: haven't implemented this one yet
unsigned int Serializer::number_of_fish = DEFAULT_NUM_OF_FISH;
float Serializer::cohesion_intensity = DEFAULT_COHESION_INTENSITY;
float Serializer::alignment_intensity = DEFAULT_ALIGNMENT_INTENSITY;
float Serializer::separation_intensity = DEFAULT_SEPARATION_INTENSITY;
float Serializer::edge_evasion_intensity = DEFAULT_EDGE_EVASION_INTENSITY;
bool Serializer::is_framerate_limited = true;
bool Serializer::use_solid_color = false;
bool Serializer::store_on_exit = true;
bool Serializer::show_gui = true;

void Serializer::loadData(){
    std::ifstream file(DATA_PATH);
    if (!file) {
        std::cout << "No data file found, using default values." << std::endl;
        storeData();
        return;
    }

    file >> fish_eye_color.r >> fish_eye_color.g >> fish_eye_color.b;
    file >> fish_fin_color.r >> fish_fin_color.g >> fish_fin_color.b;
    file >> fish_body_color.r >> fish_body_color.g >> fish_body_color.b;
    file >> fish_tex_path;
    file >> number_of_fish;
    file >> cohesion_intensity >> alignment_intensity >> separation_intensity >> edge_evasion_intensity;
    file >> is_framerate_limited >> use_solid_color >> store_on_exit >> show_gui;

    file.close();   
}

void Serializer::storeData(){
    std::ofstream file(DATA_PATH);
    if (!file) {
        std::cerr << "Error writing config file!\n";
        return;
    }

    file << fish_eye_color.r << " " << fish_eye_color.g << " " << fish_eye_color.b << "\n";
    file << fish_fin_color.r << " " << fish_fin_color.g << " " << fish_fin_color.b << "\n";
    file << fish_body_color.r << " " << fish_body_color.g << " " << fish_body_color.b << "\n";
    file << fish_tex_path << "\n";
    file << number_of_fish << "\n";
    file << cohesion_intensity << " " << alignment_intensity << " " << separation_intensity << " " << edge_evasion_intensity << "\n";
    file << is_framerate_limited << " " << use_solid_color << " " << store_on_exit << " " << show_gui << "\n";

    file.close();
}

