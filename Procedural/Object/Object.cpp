#include "Object.h"


Object::Object()
{
}
/*Builds an object using an already BUILT and FILLED VAO*/
Object::Object(GLuint *vao)
{
	this->vao = *vao;
	tessellation = false;
	point_count = 6;
}

/*
The order for textures is
layout (binding = 0) diffuse;
layout (binding = 1) specular;
layout (binding = 2) normal_map;
layout (binding = 3) cube_map;
*/
Object::Object(std::string name, bool tess, const int tex_count, Tex2D ...)
{
	tessellation = tess;
	texture_count = tex_count;
	textures = new Tex2D[tex_count];
	va_list vl;
	va_start(vl, tex_count);
	for (int i = 0; i < tex_count; i++)
	{
		textures[i] = va_arg(vl, Tex2D);
	}
	va_end(vl);

	ObjectLoader::load_mesh(name, &vao, &point_count);
}
void Object::add_shader(ShaderProgram *shader)
{
	shader = shader;
}
//pass textures here
Object::Object(std::string name)
{
	ObjectLoader::load_mesh(name, &vao, &point_count);
}


GLuint Object::get_vao() {
	return vao;
}

int Object::get_point_count() {
	return point_count;
}

void Object::render(GLuint model_matrix_location) {
	for (int i = 0; i < texture_count; i++)
	{
		textures[i].activate_texture(GL_TEXTURE0 + i);
	}
	glBindVertexArray(vao);
	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, model_matrix.get_as_array());	
	if (tessellation) {
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawArrays(GL_PATCHES, 0, point_count);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, point_count);
	}
}


Object::~Object()
{
}
