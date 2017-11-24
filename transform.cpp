#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "transform.hpp"

mat4 translateMatrix(vec3 translate) {
	return glm::translate(mat4(), translate);
}

mat4 scaleMatrix(float xs, float ys, float zs) {
	return glm::scale(vec3(xs, ys, zs));
}

mat4 rotateVector(vec3 axis, float angle) {
	return glm::rotate(angle, axis);
}

mat4 getView(vec3 camera, vec3 target, vec3 up) {
	return lookAt(camera, target, up);
}

mat4 getProjection(float fov, float ratio, float near, float far) {
	return perspective(radians(fov), ratio, near, far);
}
