#include "fishRenderer.h"

FishRenderer::FishRenderer(){
    glGenBuffers(1, &this->circleVBO);
    glGenVertexArrays(1, &this->circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->circleVBO);
    glBindVertexArray(this->circleVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glGenBuffers(1, &this->outlineVBO);
    glGenVertexArrays(1, &this->outlineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->outlineVBO);
    glBindVertexArray(this->outlineVAO);

    // the first '*2' is because vec2 has 2 float values
    int number_of_vert_positions = NUM_OF_FISH_OUTLINE_VERTS * 2;
    int number_of_vert_texel = NUM_OF_FISH_OUTLINE_VERTS * 2;

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (number_of_vert_positions + number_of_vert_texel), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(sizeof(float)*number_of_vert_positions));

    glGenBuffers(1, &this->screenQuadVBO);
    glGenVertexArrays(1, &this->screenQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->screenQuadVBO);
    glBindVertexArray(this->screenQuadVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->screenQuadVertices), this->screenQuadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));

    glGenBuffers(1, &this->waveQuadVBO);
    glGenVertexArrays(1, &this->waveQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->waveQuadVBO);
    glBindVertexArray(this->waveQuadVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->wavesQuadVertices), this->wavesQuadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    std::filesystem::path shaders_path = SHADERS_PATH;
    std::filesystem::path v_circle_shader_path = shaders_path / "v_circle.glsl";
    std::filesystem::path f_circle_shader_path = shaders_path / "f_circle.glsl";
    std::filesystem::path v_body_shader_path = shaders_path / "v_body.glsl";
    std::filesystem::path f_body_shader_path = shaders_path / "f_body.glsl";
    std::filesystem::path v_fin_shader_path = shaders_path / "v_fin.glsl";
    std::filesystem::path f_fin_shader_path = shaders_path / "f_fin.glsl";
    std::filesystem::path v_screen_shader_path = shaders_path / "v_screen.glsl";
    std::filesystem::path f_screen_shader_path = shaders_path / "f_screen.glsl";

    std::filesystem::path textures_path = TEXTURES_PATH;
    std::filesystem::path fish_texture_path = textures_path / "koi.jpg";

    
    this->fishTexture.Generate(fish_texture_path, false);

    this->screenShader = Shader(v_screen_shader_path.c_str(),
                                f_screen_shader_path.c_str());

    this->circleShader = Shader(v_circle_shader_path.c_str(),
                                f_circle_shader_path.c_str());
    
    this->bodyShader = Shader(v_body_shader_path.c_str(),
                              f_body_shader_path.c_str());

    this->finShader = Shader(v_fin_shader_path.c_str(),
                             f_fin_shader_path.c_str());


    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);

    glGenFramebuffers(1, &this->screenQuadFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->screenQuadFBO);

    glGenTextures(1, &this->screenQuadTexture);
    glBindTexture(GL_TEXTURE_2D, this->screenQuadTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mode->width, mode->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screenQuadTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

FishRenderer::~FishRenderer(){
    glDeleteBuffers(1, &this->circleVBO);
    glDeleteBuffers(1, &this->outlineVBO);
    glDeleteVertexArrays(1, &this->circleVAO);
    glDeleteVertexArrays(1, &this->outlineVAO);
}

glm::vec2 jointSidePoint(Joint& j){
    return Global::CalculateNormal(j.moveDirection) * j.circleRadius;   // returns a moveDirection vector of a joint turned -90 degrees
}

void FishRenderer::renderFish(std::vector<Fish>& allFish, Shader& circleShader, Shader& outlineShader, Shader& finShader, Shader& screenShader,
                               glm::vec2 frontFinScale, glm::vec2 backFinScale, glm::vec2 eyeScale){

    // bind to custom framebuffer object
    glBindFramebuffer(GL_FRAMEBUFFER, this->screenQuadFBO);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render pond background
    // this->renderPond(backgroundShader);

    // render the fish
    for(Fish& fish : allFish){

        this->renderFishSideFins(fish, frontFinScale, backFinScale, circleShader);
        this->renderFishBody(fish, circleShader, outlineShader);
        this->renderFishEyes(fish, eyeScale, circleShader);
        this->renderFishTailFin(fish, finShader);
        this->renderFishBackFin(fish, finShader);
    }

    // // bind to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, this->screenQuadTexture);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // // render quad
    this->renderScreenQuad(screenShader);

}

void FishRenderer::renderScreenQuad(Shader& screenShader){
    glBindVertexArray(this->screenQuadVAO);

    screenShader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->screenQuadTexture);
    glActiveTexture(GL_TEXTURE1);
    // waterNoiseTex.Bind();
    glActiveTexture(GL_TEXTURE2);
    // highlightNoiseTex.Bind();

    screenShader.setInt("screenTexture", 0);
    screenShader.setInt("waterNoiseTexture", 1);
    screenShader.setInt("highlightNoiseTexture", 2);

    glad_glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FishRenderer::renderPond(Shader& backgroundShader){

    glActiveTexture(GL_TEXTURE0);
    // backgroundTex.Bind();

    backgroundShader.use();

    backgroundShader.setInt("backgroundTex", 0);

    glBindVertexArray(waveQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void FishRenderer::renderFishBody(const Fish& fish, Shader& circleShader, Shader& outlineShader){

    glm::mat4 projection = Global::projectionMatrix;

    // this renders the first and last circle
    // this->renderOvals(fish.joints[0].Center, glm::vec2(0.0f), 0.0f, glm::vec2(fish.joints[0].circleRadius),
    //                   circleShader, fish.bodyColor, fish.joints[0].circleRadius);

    this->renderOvals(fish.joints[fish.numOfJoints-1].Center, glm::vec2(0.0f), 0.0f, glm::vec2(fish.joints[fish.numOfJoints-1].circleRadius), 
                      circleShader, fish.bodyColor, fish.joints[0].circleRadius);

    // render the outline

    glBindVertexArray(this->outlineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->outlineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * (fish.numOfJoints + 2) * sizeof(float), fish.outline_vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, 4 * (fish.numOfJoints + 2) * sizeof(float), 4 * fish.numOfJoints * sizeof(float), fish.tex_coords);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(fish.joints[0].Center, 0.0f));

    glActiveTexture(GL_TEXTURE0);
    fishTexture.Bind();

    outlineShader.use();
    outlineShader.setInt("fishTexture", 0);
    outlineShader.setMat4("projection", projection);
    outlineShader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (fish.numOfJoints + 2));

    glBindVertexArray(0);

}

void FishRenderer::renderFishBackFin(const Fish& fish, Shader& finShader){

    Joint j1 = fish.joints[fish.numOfJoints/4];     // joints at which the back fin starts and ends
    Joint j2 = fish.joints[fish.numOfJoints/2 + 1];

    glm::vec2 controlPoint = -2.5f * j1.moveDirection;

    // direction from the halfway point of the start and end joint to the control point 
    glm::vec2 controlDirection = glm::normalize(controlPoint + j1.Center - (j1.Center+j2.Center)/2.0f);

    glm::vec2 controlPoint2 = controlPoint + controlDirection;

    // Draw back fin
    this->fin_bezier.DrawCrescentBezierFilled(8, j1.Center, glm::vec2(0.0f), j2.Center - j1.Center, controlPoint, controlPoint2, finShader, fish.finColor);
}

void FishRenderer::renderFishTailFin(const Fish& fish, Shader& finShader){

    // Pretty much the same function as the renderFishBackFin

    glm::vec2 tail_fin_start = fish.joints[fish.numOfJoints-1].Center;
    glm::vec2 tail_fin_end = fish.tail_fin_joints[1].Center;

    glm::vec2 tail_fin_control1 = fish.tail_fin_joints[0].Center;
    glm::vec2 tail_fin_control_offset_dir = tail_fin_control1 - (tail_fin_start+tail_fin_end)/2.0f; 

    tail_fin_control1 = tail_fin_control1 + tail_fin_control_offset_dir * 1.5f; 
    glm::vec2 tail_fin_control2 = tail_fin_control1 + tail_fin_control_offset_dir * 5.0f;

    this->fin_bezier.DrawCrescentBezierFilled(8, tail_fin_start, glm::vec2(0.0f), tail_fin_end - tail_fin_start,
                                              tail_fin_control1 - tail_fin_start, tail_fin_control2 - tail_fin_start, finShader, fish.finColor);
}

void FishRenderer::renderOvals(glm::vec2 position, glm::vec2 offset, float rotationAngle, glm::vec2 scale, Shader& shader,
                               glm::vec3 color, float r = 0.5f){

    glBindVertexArray(this->circleVAO);

    glm::mat4 projection = Global::projectionMatrix;
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position + offset, 0.0f));
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("model", model);
    shader.setVec3("color", color);
    shader.setFloat("r", r);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void FishRenderer::renderFishSideFins(const Fish& fish, glm::vec2 frontScale, glm::vec2 backScale, Shader& finShader){

    Joint frontFinsJoint = fish.joints[fish.numOfJoints/6];   
    Joint backFinsJoint  = fish.joints[fish.numOfJoints/2+1];   

    glm::vec2 frontOffset = jointSidePoint(frontFinsJoint);
    glm::vec2 backOffset  = jointSidePoint(backFinsJoint);

    // rotates the fin corresponding the move direction of the joint it is attached to, then an offset it applied so the fin isn't sticking straigh out
    float rightFinAngle = Global::angleOfVector(frontFinsJoint.moveDirection) - 3.0f*PI/4.0f;
    // same angle between the find and move direction but the offset should be the opposite from the first fin
    float leftFinAngle  = rightFinAngle + 6.0f*PI/4.0f;

    this->renderOvals(frontFinsJoint.Center, frontOffset, rightFinAngle, frontScale, finShader, fish.finColor);
    this->renderOvals(frontFinsJoint.Center, -frontOffset, leftFinAngle, frontScale, finShader, fish.finColor);

    rightFinAngle = Global::angleOfVector(backFinsJoint.moveDirection) - 7.0f*PI/8.0f;
    leftFinAngle  = rightFinAngle + 14.0f*PI/8.0f;

    this->renderOvals(backFinsJoint.Center, backOffset, rightFinAngle, backScale, finShader, fish.finColor);
    this->renderOvals(backFinsJoint.Center, -backOffset, leftFinAngle, backScale, finShader, fish.finColor);
}

void FishRenderer::renderFishEyes(const Fish& fish, glm::vec2 scale, Shader& circleShader){

    // pretty much the same stuff as the side fins

    Joint headJoint = fish.joints[0];

    glm::vec2 offset = jointSidePoint(headJoint) * 0.85f;

    float rightEyeAngle = Global::angleOfVector(headJoint.moveDirection) - PI/2.0f + PI/20.0f;
    float leftEyeAngle  = Global::angleOfVector(headJoint.moveDirection) + PI/2.0f - PI/20.0f;

    // the only difference is that we're first rendering an oval of the same color as the fish body so it's not like the eye is sticking out or something

    // this->renderOvals(headJoint.Center, offset, rightEyeAngle, scale * 1.4f, circleShader, fish.bodyColor);     // right eye
    this->renderOvals(headJoint.Center, offset, rightEyeAngle, scale, circleShader, fish.eyeColor);             // right eye

    // this->renderOvals(headJoint.Center, -offset, leftEyeAngle, scale * 1.4f, circleShader, fish.bodyColor);     // left eye
    this->renderOvals(headJoint.Center, -offset, leftEyeAngle, scale, circleShader, fish.eyeColor);             // left  eye
}















