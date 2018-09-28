#pragma once
//include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Math/Vector/Vector3f.h"
#include "Math/Matrix/Matrix4f.h"
#include "Log/Log.h"

struct WindowDimensions
{
	float width;
	float height;
};

struct Development
{
	bool use_wireframe = false;
	bool use_framebuffers = true;
};

//generic quad to screen
static GLuint generic_quad_vao, generic_quad_vp_vbo;
static bool generic_quad_built = false;
const static float points[] = {
	  -1.0f, -1.0f,
	   1.0f, -1.0f,
	  -1.0f,  1.0f,
	  -1.0f,  1.0f,
	   1.0f, -1.0f,
	   1.0f,  1.0f
};

void _build_generic_quad();