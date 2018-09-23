#include "Tex2D.h"



Tex2D::Tex2D()
{
}

Tex2D::Tex2D(std::string name, GLenum type)
{
	load_texture(name, type);
}

GLuint Tex2D::load_texture(std::string name, GLenum type)
{
	unsigned char* image_data;
	this->type = type;
	if (type == GL_TEXTURE_CUBE_MAP) {
		name = "Resources/Textures/skybox/" + name;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex_index);
		load_cubemap(tex_index, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, name + "_negz.jpg");
		load_cubemap(tex_index, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, name + "_posz.jpg");
		load_cubemap(tex_index, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, name + "_negy.jpg");
		load_cubemap(tex_index, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, name + "_posy.jpg");
		load_cubemap(tex_index, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, name + "_negx.jpg");
		load_cubemap(tex_index, GL_TEXTURE_CUBE_MAP_POSITIVE_X, name + "_posx.jpg");
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);			
	}
	else {
		name = "Resources/Textures/" + name;
		image_data = stbi_load(name.c_str(), &width, &height, &size, 4);
		rotate_texture(image_data);
		glGenTextures(1, &tex_index);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_index);
		GLfloat max_aniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
		delete[] image_data;
	}	
	
	return tex_index;
}

bool Tex2D::load_cubemap(GLuint tex_index, GLenum side, std::string name) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_index);
	unsigned char* image_data = stbi_load(name.c_str(), &width, &height, &size, 4);

	glTexImage2D(side, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	delete[] image_data;
	return true;
}

void Tex2D::activate_texture(GLenum sampler) {
	glActiveTexture(sampler);
	glBindTexture(type, tex_index);
}

void Tex2D::rotate_texture(unsigned char* image_data) {
	int byte_width = width * 4;
	int half_height = height / 2;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char aux = 0;

	for (int i = 0; i < half_height; i++)
	{
		top = image_data + i * byte_width;
		bottom = image_data + (height - i - 1) * byte_width;
		for (int j = 0; j < byte_width; j++)
		{
			aux = *top;
			*top = *bottom;
			*bottom = aux;
			top++;
			bottom++;
		}
	}
}

Tex2D::~Tex2D()
{
}
