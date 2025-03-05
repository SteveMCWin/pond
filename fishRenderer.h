#ifndef FISH_RENDERER_H
#define FISH_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "fish.h"
#include "shader.h"
#include "bezier_filled.h"
#include "texture.h"

class FishRenderer{

public:
     FishRenderer();    // responsible for creating the VBOs and VAOs needed to render the fish
    ~FishRenderer();

    void renderFish(std::vector<Fish>& allFish, Shader& circleShader, Shader& outlineShader, Shader& finShader, Shader& screenShader,
                    Shader& backgroundShader, glm::vec2 frontFinScale, glm::vec2 backFinScale, glm::vec2 eyeScale);

private:

    void renderScreenQuad(Shader& screenShader);

    void renderFishBody(const Fish& fish, Shader& outlineShader);
    void renderFishBackFin(const Fish& fish, Shader& finShader);
    void renderFishTailFin(const Fish& fish, Shader& finShader);
    void renderFishSideFins(const Fish& fish, glm::vec2 frontScale, glm::vec2 backScale, Shader& finShader);
    void renderFishEyes(const Fish& fish, glm::vec2 scale, Shader& circleShader);

    void renderOvals(glm::vec2 position, glm::vec2 offset, float rotationAngle, glm::vec2 scale, Shader& shader, glm::vec3 color, float r);

    void renderPond(Shader& backgroundShader);

    float vertices[12] = {  // quad vertices used for rendering ovals
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

    float wavesQuadVertices[24] = {
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
    unsigned int waveQuadVBO, waveQuadVAO;

    unsigned int screenQuadFBO;
    unsigned int multisampledTex;
    unsigned int screenQuadTexture;

    // Texture2D backgroundTex;
    // Texture2D waterNoiseTex;
    // Texture2D highlightNoiseTex;
    Texture2D fishTexture;

};

#endif // !FISH_RENDERER_H

