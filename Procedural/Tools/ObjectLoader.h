#pragma once
#pragma once
//include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h>


#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdlib.h> // memory management
#include <vector>
#include "../Math/Vector/Vector3f.h"

static class ObjectLoader
{
public:
	ObjectLoader();
	static std::vector<GLfloat> calculate_normals(const float * points, const int size);
	static bool load_mesh(std::string file_name, GLuint * vao, int * point_count);
	~ObjectLoader();
};

