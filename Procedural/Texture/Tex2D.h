#pragma once
//include GLEW
#include "../CommonHeaders.h"

#include "../Libs/stb_image.h"

class Tex2D
{
public:
	Tex2D();
	Tex2D(std::string name, GLenum type);
	void activate_texture(GLenum sampler);
	
	~Tex2D();
private:
	GLuint load_texture(std::string name, GLenum type);
	void rotate_texture();
	unsigned char* image_data;
	GLuint tex_index;
	int width, height, size;
};

