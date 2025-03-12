#include "serializer.h"

glm::vec3 Serializer::fin_color = DEFAULT_FIN_COLOR;
std::string Serializer::custom_tex_path = (std::filesystem::path)TEXTURES_PATH / "koi.jpg";
unsigned int Serializer::number_of_fish = DEFAULT_NUM_OF_FISH;
float Serializer::cohesion_intensity = DEFAULT_COHESION_INTENSITY;
float Serializer::alignment_intensity = DEFAULT_ALIGNMENT_INTENSITY;
float Serializer::separation_intensity = DEFAULT_SEPARATION_INTENSITY;
float Serializer::edge_evasion_intensity = DEFAULT_EDGE_EVASION_INTENSITY;
bool Serializer::is_framerate_limited = true;
bool Serializer::use_texture = true;

void Serializer::loadData(){
    std::ifstream file(DATA_PATH);
    if (!file) {
        std::cout << "No data file found, using default values.\n";
        return;
    }

    file >> fin_color.r >> fin_color.g >> fin_color.b;
    file >> custom_tex_path;
    file >> number_of_fish;
    file >> cohesion_intensity >> alignment_intensity >> separation_intensity >> edge_evasion_intensity;
    file >> is_framerate_limited >> use_texture;

    file.close();   
}

void Serializer::storeData(){
    std::ofstream file(DATA_PATH);
    if (!file) {
        std::cerr << "Error writing config file!\n";
        return;
    }

    file << fin_color.r << " " << fin_color.g << " " << fin_color.b << "\n";
    file << custom_tex_path << "\n";
    file << number_of_fish << "\n";
    file << cohesion_intensity << " " << alignment_intensity << " " << separation_intensity << " " << edge_evasion_intensity << "\n";
    file << is_framerate_limited << " " << use_texture << "\n";

    file.close();
}


void Serializer::setFinColor(const glm::vec3& color) { fin_color = color; }

void Serializer::setCustomTexPath(const std::string& path) { custom_tex_path = path; }

void Serializer::setNumberOfFish(unsigned int num) { number_of_fish = num; }

void Serializer::setCohesionIntensity(float value) { cohesion_intensity = value; }

void Serializer::setAlignmentIntensity(float value) { alignment_intensity = value; }

void Serializer::setSeparationIntensity(float value) { separation_intensity = value; }

void Serializer::setEdgeEvasionIntensity(float value) { edge_evasion_intensity = value; }

void Serializer::setIsFramerateLimited(bool is_limited) { is_framerate_limited = is_limited; }

void Serializer::setUseTexture(bool value) { use_texture = value; }



glm::vec3 Serializer::getFinColor() { return fin_color; }

std::string Serializer::getCustomTexPath() { return custom_tex_path; }

unsigned int Serializer::getNumberOfFish() { return number_of_fish; }

float Serializer::getCohesionIntensity() { return cohesion_intensity; }

float Serializer::getAlignmentIntensity() { return alignment_intensity; }

float Serializer::getSeparationIntensity() { return separation_intensity; }

float Serializer::getEdgeEvasionIntensity() { return edge_evasion_intensity; }

bool Serializer::getIsFramerateLimited() { return is_framerate_limited; }

bool Serializer::getUseTexture() { return use_texture; }
