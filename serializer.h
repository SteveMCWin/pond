#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "global.h"

class Serializer{
public:
     Serializer();
    ~Serializer();

    void setFinColor(const glm::vec3& color);
    void setCustomTexPath(const std::string& path);
    void setFramerateCap(unsigned int cap);
    void setNumberOfFish(unsigned int num);
    void setCohesionIntensity(float value);
    void setAlignmentIntensity(float value);
    void setSeparationIntensity(float value);
    void setEdgeEvasionIntensity(float value);
    void setUseTexture(bool value);

    glm::vec3 getFinColor() const;
    std::string getCustomTexPath() const;
    unsigned int getFramerateCap() const;
    unsigned int getNumberOfFish() const;
    float getCohesionIntensity() const;
    float getAlignmentIntensity() const;
    float getSeparationIntensity() const;
    float getEdgeEvasionIntensity() const;
    bool getUseTexture() const;
    /* things i want in settings:
     * fish fin colors
     * fish texture path
     * framerate
     * num of fish
     * boids params
    */
private:
    glm::vec3 fin_color;
    std::string custom_tex_path;
    unsigned int framerate_cap;
    unsigned int number_of_fish;
    float cohesion_intensity;
    float alignment_intensity;
    float separation_intensity;
    float edge_evasion_intensity;
    bool use_texture;
};
