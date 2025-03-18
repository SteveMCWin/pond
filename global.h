#ifndef GLOBAL_H
#define GLOBAL_H

#include <glad/glad.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>

#ifndef DEFS
#define DEFS

#define PI 3.1415926535f
#define FRUSTUM_HALF_WIDTH 80.0f
#define MAX_NUM_OF_FISH 50
#define NUM_OF_JOINTS 12
#define FISH_SIGHT_RANGE 25.0f
#define NUM_OF_FISH_OUTLINE_VERTS ((NUM_OF_JOINTS+2)*2)
#define NUM_OF_HIT_CHECKS 13
#define HIT_CHECK_ANGLE 120.0f
#define FISH_SPEED 15.0f
#define FRONT_FIN_SCALE glm::vec2(1.5f, 0.5f)
#define BACK_FIN_SCALE glm::vec2(1.5f, 0.5f)
#define EYE_SCALE glm::vec2(0.15f, 0.4f)
#define DEFAULT_EYE_COLOR glm::vec3(0.1f, 0.2f, 0.2f)
#define DEFAULT_FIN_COLOR glm::vec3(0.96f, 0.43f, 0.23f)
#define DEFAULT_BODY_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define DEFAULT_NUM_OF_FISH 15
#define DEFAULT_COHESION_INTENSITY 0.5f
#define DEFAULT_ALIGNMENT_INTENSITY 180.0f
#define DEFAULT_SEPARATION_INTENSITY 180.0f
#define DEFAULT_EDGE_EVASION_INTENSITY 80.0f
#define DEFAULT_PIXELATION_AMOUNT 512.0f
#define DEFAULT_IS_FRAMERATE_LIMITED true
#define DEFAULT_USE_PIXELATION false
#define DEFAULT_STORE_ON_EXIT true
#define DEFAULT_SHOW_GUI true
#define DEFAULT_USE_SOLID_COLOR false

#endif // !DEFS

class Global{
public:
    static float aspect_ratio;
    static glm::mat4 projectionMatrix;

    static float DegToRad(float degrees);   // converts degrees to radians
    static glm::vec2 CalculateNormal(glm::vec2 vec);    // returns a normal vector pointing to the right if the input vec is pointing up
    static float GetRandomFloat();  // returns float ranging from 0.0 to 1.0
    static float angleOfVector(glm::vec2 v);   // returns the angle in radians between the input angle and the <1.0, 0.0> vector (return value ranges from -pi to pi)
    static glm::vec2 rotateVector(glm::vec2& vector, float angle);   // rotates a vector a specified amount of degrees
};

#endif // !GLOBAL_H
