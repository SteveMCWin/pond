#include "global.h"
#include "serializer.h"

Serializer::Serializer(){
    std::ifstream file(DATA_PATH);
    if (!file) {
        std::cout << "No config file found, using default values.\n";

        fin_color = DEFAULT_FIN_COLOR;
        custom_tex_path = (std::filesystem::path)TEXTURES_PATH / "koi.jpg";
        framerate_cap = 60;
        number_of_fish = DEFAULT_NUM_OF_FISH;
        cohesion_intensity = DEFAULT_COHESION_INTENSITY;
        alignment_intensity = DEFAULT_ALIGNMENT_INTENSITY;
        separation_intensity = DEFAULT_SEPARATION_INTENSITY;
        edge_evasion_intensity = DEFAULT_EDGE_EVASION_INTENSITY;
        use_texture = true;
        return;
    }

    file >> fin_color.r >> fin_color.g >> fin_color.b;
    file >> custom_tex_path;
    file >> framerate_cap >> number_of_fish;
    file >> cohesion_intensity >> alignment_intensity >> separation_intensity >> edge_evasion_intensity;
    file >> use_texture;

    file.close();
}

Serializer::~Serializer(){
    std::ofstream file(DATA_PATH);
    if (!file) {
        std::cerr << "Error writing config file!\n";
        return;
    }

    file << fin_color.r << " " << fin_color.g << " " << fin_color.b << "\n";
    file << custom_tex_path << "\n";
    file << framerate_cap << " " << number_of_fish << "\n";
    file << cohesion_intensity << " " << alignment_intensity << " " << separation_intensity << " " << edge_evasion_intensity << "\n";
    file << use_texture << "\n";

    file.close();
}

void Serializer::setFinColor(const glm::vec3& color) { fin_color = color; }

void Serializer::setCustomTexPath(const std::string& path) { custom_tex_path = path; }

void Serializer::setFramerateCap(unsigned int cap) { framerate_cap = cap; }

void Serializer::setNumberOfFish(unsigned int num) { number_of_fish = num; }

void Serializer::setCohesionIntensity(float value) { cohesion_intensity = value; }

void Serializer::setAlignmentIntensity(float value) { alignment_intensity = value; }

void Serializer::setSeparationIntensity(float value) { separation_intensity = value; }

void Serializer::setEdgeEvasionIntensity(float value) { edge_evasion_intensity = value; }

void Serializer::setUseTexture(bool value) { use_texture = value; }

glm::vec3 Serializer::getFinColor() const { return fin_color; }

std::string Serializer::getCustomTexPath() const { return custom_tex_path; }

unsigned int Serializer::getFramerateCap() const { return framerate_cap; }

unsigned int Serializer::getNumberOfFish() const { return number_of_fish; }

float Serializer::getCohesionIntensity() const { return cohesion_intensity; }

float Serializer::getAlignmentIntensity() const { return alignment_intensity; }

float Serializer::getSeparationIntensity() const { return separation_intensity; }

float Serializer::getEdgeEvasionIntensity() const { return edge_evasion_intensity; }

bool Serializer::getUseTexture() const { return use_texture; }
