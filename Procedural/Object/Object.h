#pragma once
#include "../CommonHeaders.h"

#include "../Math/Matrix/Matrix4f.h"
class Object
{
public:
	Object();
	~Object();
private:
	Matrix4f model_matrix;
	GLuint vao;
};

