#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>
using std::string;
using std::ifstream;

#include <GL/glew.h>

#include "shader.hpp"

GLuint loadShaders(const char *vsPath, const char *fsPath) {

	// create shaders
	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

	// read vertex shader
	string vsCode;
	ifstream vsStream(vsPath, std::ios::in);
	if (vsStream.is_open()) {
		string line = "";
		while (getline(vsStream, line))
			vsCode += "\n" + line;
		vsStream.close();
	}
	else {
		printf("couldn't find vertex shader in specified path: %s\n", vsPath);
		getchar();
		return 0;
	}

	// read fragment shader
	string fsCode;
	ifstream fsStream(fsPath, std::ios::in);
	if (fsStream.is_open()) {
		string line = "";
		while (getline(fsStream, line))
			fsCode += "\n" + line;
		fsStream.close();
	}
	else {
		printf("couldn't find fragment shader in specified path: %s\n", fsPath);
		getchar();
		return 0;
	}

	GLint result = GL_FALSE;
	int infolog;

	// compile vertex shader
	printf("compiling vertex shader: %s\n", vsPath);
	char const *vsourcePtr = vsCode.c_str();
	glShaderSource(vsID, 1, &vsourcePtr, NULL);
	glCompileShader(vsID);

	// check vertex shader
	glGetShaderiv(vsID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &infolog);
	if (infolog > 0) {
		vector<char> vsErrorMsg(infolog + 1);
		glGetShaderInfoLog(vsID, infolog, NULL, &vsErrorMsg[0]);
		printf("%s\n", &vsErrorMsg[0]);
	}

	// compile fragment shader
	printf("compiling fragment shader: %s\n", fsPath);
	char const *fsourcePtr = fsCode.c_str();
	glShaderSource(fsID, 1, &fsourcePtr, NULL);
	glCompileShader(fsID);

	// check fragment shader
	glGetShaderiv(fsID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &infolog);
	if (infolog > 0) {
		vector<char> fsErrorMsg(infolog + 1);
		glGetShaderInfoLog(fsID, infolog, NULL, &fsErrorMsg[0]);
		printf("%s\n", &fsErrorMsg[0]);
	}

	// link program
	printf("linking program\n");
	GLuint progID = glCreateProgram();
	glAttachShader(progID, vsID);
	glAttachShader(progID, fsID);
	glLinkProgram(progID);

	// check program
	glGetProgramiv(progID, GL_LINK_STATUS, &result);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infolog);
	if (infolog > 0) {
		vector<char> progErrorMsg(infolog + 1);
		glGetProgramInfoLog(progID, infolog, NULL, &progErrorMsg[0]);
		printf("%s\n", &progErrorMsg[0]);
	}

	// delete everything so it's clean
	glDetachShader(progID, vsID);
	glDetachShader(progID, fsID);
	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return progID;
}