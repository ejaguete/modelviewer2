#include <glfw3.h>
extern GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "camera.hpp"
#include "transform.hpp"

const float PI = 3.14f;

mat4 view;
mat4 projection;

mat4 getCamView() {
	return view;
}

mat4 getCamProjection() {
	return projection;
}

// initial position +z
vec3 position = vec3(0, 0, 5);
float h_angle = PI;
float v_angle = 0.0f;
float init_fov = 45.0f;

float spd = 3.0f;
float mouse_spd = 0.005f;

void computeFromInputs() {
	static double last = glfwGetTime();

	// differences from current and last frame
	double current = glfwGetTime();
	float delta = float(current - last);

	// mouse pos
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	// put mouse back in centre
	glfwSetCursorPos(window, 1024/2, 768/2);

	// compute new orientation
	h_angle += mouse_spd * float(1024 / 2 - x);
	v_angle += mouse_spd * float(768 / 2 - y);

	// polar > cartesian
	vec3 direction(
		cos(v_angle) * sin(h_angle),
		sin(v_angle),
		cos(v_angle) * cos(h_angle)
	);

	// vector to move right
	vec3 right = vec3(
		sin(h_angle - PI / 2.0f),
		0,
		cos(h_angle - PI / 2.0f)
	);

	// vector to move up
	vec3 up = cross(right, direction);


	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * delta * spd;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * delta * spd;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * delta * spd;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * delta * spd;
	}

	float fov = init_fov;

	// projection matrix
	projection = getProjection(fov, 4.0f / 3.0f, 0.1f, 100.0f);
	// view
	view = getView(position, position + direction, up);

	last = current;
}

