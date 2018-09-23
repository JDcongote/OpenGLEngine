#include "Object.h"



Object::Object()
{
}
//pass textures here
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
	unsigned int tex_enum = GL_TEXTURE0;
	for (int i = 0; i < texture_count; i++)
	{
		textures[i].activate_texture(tex_enum);
		tex_enum++;
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
