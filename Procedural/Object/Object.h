#pragma once
#include "../CommonHeaders.h"
#include "../Tools/ObjectLoader.h"

#include "../Math/Matrix/Matrix4f.h"
#include "../Texture/Tex2D.h"
class Object
{
public:
	Object();
	Object(std::string name);
	Object(std::string name, bool tess, const int tex_count, Tex2D ...);

	void render(GLuint model_matrix_location);

	GLuint get_vao();	
	Matrix4f model_matrix;
	int get_point_count();
	~Object();
private:	
	bool tessellation;
	Tex2D* textures;
	GLuint vao;
	int point_count, texture_count;
};

