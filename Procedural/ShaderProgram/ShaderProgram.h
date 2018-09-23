#pragma once
#include "../CommonHeaders.h"

class ShaderProgram
{
	struct shader
	{
		std::string name;
		GLuint id;
		std::string source;
		GLenum type;
	};
public:
	Log log;
	ShaderProgram();
	ShaderProgram(Log log);
	void load_shader(std::string name, GLenum type);
	void attach_and_link(GLuint program);
	// Keep track of this variable the rest can be deleted
	GLuint program_index;
	void reload_shaders();
	~ShaderProgram();
private:
	std::string read_shader(std::string name);
	void _print_shader_log(GLuint shader_index);
	void _check_compile_errors(GLuint compiled_shader);
	void _print_shader_program_log(GLuint program);
	const char * GL_type_to_string(GLenum type);
	void _print_all_shader_info(GLuint program);
	bool _is_valid(GLuint program);
	void _check_link_errors(GLuint program);

	shader vertex_shader = { "", 0};
	shader fragment_shader = { "", 0 };
	shader geometry_shader = { "", 0 };
	shader tesscontrol_shader = { "", 0 };
	shader tesseval_shader = { "", 0 };

	shader shaders[5];

	int shader_count = 0;
};

