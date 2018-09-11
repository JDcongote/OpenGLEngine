#pragma once
//include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "../Log/Log.h"

class ShaderProgram
{
	struct shader
	{
		std::string name;
		GLuint id;
	};
public:
	Log log;
	ShaderProgram();
	ShaderProgram(Log log);
	void load_shader(std::string name, GLenum type);
	void attach_and_link();
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
	shader vertex_shader;
	shader fragment_shader;
	shader geometry_shader;
};

