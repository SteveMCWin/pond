#ifndef FISH_RENDERER_H
#define FISH_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <cmath>

#include "fish.h"
#include "shader.h"
#include "bezier_filled.h"
#include "texture.h"
#include "global.h"
#include "serializer.h"

class FishRenderer{

public:
     FishRenderer();    // responsible for creating the VBOs and VAOs needed to render the fish
    ~FishRenderer();

    void renderFish(Fish* allFish, unsigned int number_of_fish);

    void update_texture_path();
    void update_fish_eye_color();
    void update_fish_fin_color();
    void update_fish_body_color();
    void update_use_solid_color();
    void update_use_pixelation();
    void update_pixelation_amount();

private:
    glm::vec2 jointSidePoint(Joint &j);

    void renderScreenQuad();

    void renderFishBody(const Fish& fish);
    void renderFishBackFin(const Fish& fish);
    void renderFishTailFin(const Fish& fish);
    void renderFishSideFins(const Fish& fish);
    void renderFishEyes(const Fish& fish);

    void renderOvals(glm::vec2 position, glm::vec2 offset, float rotationAngle, glm::vec2 scale, glm::vec3 color, float r);

    float circle_vertices[12] = {  // quad vertices used for rendering ovals
        -1.0, -1.0,
         1.0, -1.0,
        -1.0,  1.0,

         1.0, -1.0,
         1.0,  1.0,
        -1.0,  1.0
    };

    float screenQuadVertices[24] = {    // quad vertices used for rendering the fbo texture to the screen
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    BezierCurve fin_bezier; // bezier curve class object used for rendering fins

    unsigned int circleVBO, circleVAO;
    unsigned int outlineVBO, outlineVAO;
    unsigned int screenQuadVBO, screenQuadVAO;

    unsigned int screenQuadFBO;

    Texture2D screenQuadTexture;

    Texture2D fishTexture;

    Shader circleShader;
    Shader bodyShader;
    Shader finShader;
    Shader screenShader;

    glm::vec3 fishEyeColor;
    glm::vec3 fishFinColor;
    glm::vec3 fishBodyColor;

    float pixelationAmount;

    bool useSolidColor;
    bool usePixelation;

};

#endif // !FISH_RENDERER_H

