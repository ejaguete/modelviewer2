#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

mat4 getCamView();
mat4 getCamProjection();

void computeFromInputs();

#endif