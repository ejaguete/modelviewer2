#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
using std::vector;

#include <GL/glew.h>
#include <glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "headers/shader.hpp"
#include "headers/transform.hpp"
#include "headers/loadobj.hpp"
#include "headers/camera.hpp"
#include "headers/loadtex.hpp"


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	// in case glew doesn't want to work
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// turn sticky keys on
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// hide mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// put mouse in centre of screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// set bg color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	// accept frag if it's closer to camera than the previous one

	///////////////////////
	// SET EVERYTHING UP //
	///////////////////////

	// VAO
	// always set this up first
	GLuint va;
	glGenVertexArrays(1, &va);		// make one vertex array with id "va"
	glBindVertexArray(va);			// set array as current one

	// create shader program
	GLuint progID = loadShaders("vertexShader.glsl", "fragShader.glsl");

	// get mvp handle
	GLuint matID = glGetUniformLocation(progID, "mvp");

	// VBO
	// load obj file
	vector<vec3> vs;
	vector<vec2> uvs;
	vector<vec3> norms;
	bool res = loadobj("timmy_cup/timmy_cup.obj", vs, uvs, norms);

	GLuint vbuf;
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(vec3), &vs[0], GL_STATIC_DRAW);

	GLuint uvbuf;
	glGenBuffers(1, &uvbuf);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuf);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

	do{
		// clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use shaders
		glUseProgram(progID);

		// compute MVP matrix from keyboard/mouse
		computeFromInputs();
		mat4 model = mat4(1.0f);
		mat4 proj = getCamProjection();
		mat4 view = getCamView();
		mat4 mvp = proj * view * model;

		glUniformMatrix4fv(matID, 1, GL_FALSE, &mvp[0][0]);

		// 1st attrib buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// config vertex array

		// 2nd attrib buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuf);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	// config vertex array

																		// 2nd attrib buffer : UVs
		// draw
		glDrawArrays(GL_TRIANGLES, 0, vs.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vbuf);
	glDeleteBuffers(1, &uvbuf);
	glDeleteProgram(progID);
	glDeleteVertexArrays(1, &va);

	// close everything
	glfwTerminate();

	return 0;
}

