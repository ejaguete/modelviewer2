#include <vector>
#include <glm/glm.hpp>
using std::vector;
using std::string;
using namespace glm;

#include "headers/loadobj.hpp"

bool loadobj(const char *path,
	vector<vec3> &outVerts,
	vector<vec2> &outUVs,
	vector<vec3> outNorms) {

	vector<unsigned int> vIndices, uvIndices, normIndices;
	vector<vec3> tempVerts;
	vector<vec2> tempUVs;
	vector<vec3> tempNorms;

	printf("loading obj file %s: ", path);

	FILE *file = fopen(path, "r");
	if (file == NULL) {
		printf("cannot open file: %s\n", path);
		return false;
	}

	while (true) {
		char line[128];	// buffer
		int res = fscanf(file, "%s", line);
		if (res == EOF)
			break;

		// parse line
		if (strcmp(line, "v") == 0) {
			vec3 v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
			tempVerts.push_back(v);
		}
		else if (strcmp(line, "vt") == 0) {
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUVs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0) {
			vec3 n;
			fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
			tempNorms.push_back(n);
		}
		else if (strcmp(line, "f") == 0) {
			unsigned int vIndex[3], uvIndex[3], normIndex[3];
			int match = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vIndex[0], &uvIndex[0], &normIndex[0],
				&vIndex[1], &uvIndex[1], &normIndex[1],
				&vIndex[2], &uvIndex[2], &normIndex[2]);
			if (match != 9) {
				printf("error reading file");
				fclose(file);
				return false;
			}

			for (unsigned int i = 0; i < 3; i++) {
				vIndices.push_back(vIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normIndices.push_back(normIndex[i]);
			}
		}
		else {
			// skip line we don't care about
			char eatBuffer[1000];
			fgets(eatBuffer, 1000, file);
		}
	}
	// done reading & populating internal stuff

	// populate external vectors
	for (unsigned int i = 0; i < vIndices.size(); i++) {
		unsigned int vIndex = vIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normIndex = normIndices[i];

		vec3 v = tempVerts[vIndex - 1];
		vec2 uv = tempUVs[uvIndex - 1];
		vec3 n = tempNorms[normIndex - 1];

		outVerts.push_back(v);
		outUVs.push_back(uv);
		outNorms.push_back(n);
	}
	fclose(file);
	return true;
}