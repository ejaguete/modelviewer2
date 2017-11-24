#ifndef LOADOBJ_HPP
#define LOADOBJ_HPP

#include <glm/glm.hpp>
#include <vector>
using namespace glm;
using std::vector;

bool loadobj(const char *path,
	vector<vec3> &outVerts,
	vector<vec2> &outUVs,
	vector<vec3> outNorms);

#endif
