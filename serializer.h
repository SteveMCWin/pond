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

    static void setFinColor(const glm::vec3& color);
    static void setCustomTexPath(const std::string& path);
    static void setNumberOfFish(unsigned int num);
    static void setCohesionIntensity(float value);
    static void setAlignmentIntensity(float value);
    static void setSeparationIntensity(float value);
    static void setEdgeEvasionIntensity(float value);
    static void setIsFramerateLimited(bool is_limited);
    static void setUseTexture(bool value);

    static glm::vec3 getFinColor();
    static std::string getCustomTexPath();
    static unsigned int getNumberOfFish();
    static float getCohesionIntensity();
    static float getAlignmentIntensity();
    static float getSeparationIntensity();
    static float getEdgeEvasionIntensity();
    static bool getIsFramerateLimited();
    static bool getUseTexture();
    
private:
    static glm::vec3 fin_color;
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
