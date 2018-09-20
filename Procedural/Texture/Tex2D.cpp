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
	name = "Resources/Textures/" + name;
	image_data = stbi_load(name.c_str(), &width, &height, &size, 4);
	rotate_texture();
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

	return tex_index;
}

void Tex2D::activate_texture(GLenum sampler) {
	glActiveTexture(sampler);
	glBindTexture(GL_TEXTURE_2D, tex_index);
}

void Tex2D::rotate_texture() {
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
