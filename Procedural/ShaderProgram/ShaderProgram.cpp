#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
	program_index = glCreateProgram();
}

ShaderProgram::ShaderProgram(Log log)
{
	this->log = log;
	program_index = glCreateProgram();
}

std::string ShaderProgram::read_shader(std::string name) {
	std::string shader;

	name = "Resources/Shaders/" + name;
	std::string line;
	std::ifstream file(name);
	if (file.is_open()) {
		while (getline(file, line)) {
			shader += line + '\n';
		}
	}
	log.gl_log("\nSHADER: read shader: \n");
	log.gl_log(name.c_str());
	log.gl_log("\n");

	return shader;
}

void ShaderProgram::_print_shader_log(GLuint shader_index) {
	int max_length = 0;
	char *shader_log;	
	int actual_length = 0;
	glGetShaderiv(shader_index, GL_INFO_LOG_LENGTH, &max_length);
	shader_log = (char *)malloc(max_length);
	glGetShaderInfoLog(shader_index, max_length, &actual_length, shader_log);
	printf("shader info log for GL index %u:\n%s\n", shader_index, shader_log);
	free(shader_log);
}

void ShaderProgram::_check_compile_errors(GLuint compiled_shader) {
	int params = -1;
	glGetShaderiv(compiled_shader, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		log.gl_log_err("\nERROR: Shader, %i, didn't compile\n", compiled_shader);
		fprintf(stderr, "\nERROR: Shader, %i, didn't compile\n", compiled_shader);
		_print_shader_log(compiled_shader);
	}
}

void ShaderProgram::_print_shader_program_log(GLuint program) {
	const int max_length = 2048;
	int actual_length = 0;
	char shader_log[max_length];
	glGetProgramInfoLog(program, max_length, &actual_length, shader_log);
	printf("Program info log for GL index %u:\n%s\n", program, shader_log);
}

const char* ShaderProgram::GL_type_to_string(GLenum type) {
	switch (type) {
	case GL_BOOL: return "bool";
	case GL_INT: return "int";
	case GL_FLOAT: return "float";
	case GL_FLOAT_VEC2: return "vec2";
	case GL_FLOAT_VEC3: return "vec3";
	case GL_FLOAT_VEC4: return "vec4";
	case GL_FLOAT_MAT2: return "mat2";
	case GL_FLOAT_MAT3: return "mat3";
	case GL_FLOAT_MAT4: return "mat4";
	case GL_SAMPLER_2D: return "sampler2D";
	case GL_SAMPLER_3D: return "sampler3D";
	case GL_SAMPLER_CUBE: return "samplerCube";
	case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
	default: break;
	}
	return "other";
}
/*
Prints all shader info, uniforms attributes and linked shaders
*/
void ShaderProgram::_print_all_shader_info(GLuint program) {
	log.gl_log("\n\n--------------------\nshader program %i info:\n", program);
	int params = -1;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
	log.gl_log("GL_ATTACHED_SHADERS = %i\n", params);

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
	log.gl_log("GL_ACTIVE_ATTRIBUTES = %i\n", params);

	for (GLuint i = 0; i < (GLuint)params; i++) {
		const int max_length = 64;
		char name[max_length];
		int actual_length = 0;
		int size = 0;
		GLenum type;

		glGetActiveAttrib(program, i, max_length, &actual_length, &size, &type, name);

		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64];
				log.gl_log(long_name, "%s[%i]", name, j);

				int location = glGetAttribLocation(program, long_name);
				log.gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), long_name, location);
			}
		}
		else {
			int location = glGetAttribLocation(program, name);
			log.gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), name, location);
		}
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
	log.gl_log("GL_ACTIVE_UNIFORMS = %i\n", params);

	for (GLuint i = 0; i < (GLuint)params; i++) {
		const int max_length = 64;
		char name[max_length];
		int actual_length = 0;
		int size = 0;
		GLenum type;

		glGetActiveUniform(program, i, max_length, &actual_length, &size, &type, name);

		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64];
				log.gl_log(long_name, "%s[%i]", name, j);

				int location = glGetUniformLocation(program, long_name);
				log.gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), long_name, location);
			}
		}
		else {
			int location = glGetUniformLocation(program, name);
			log.gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), name, location);
		}
	}
}

/*
Disable for release, expensive function
*/
bool ShaderProgram::_is_valid(GLuint program) {
	glValidateProgram(program);
	int params = -1;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
	log.gl_log("program %i GL_VALIDATE_STATUS = %i\n\n", program, params);
	if (GL_TRUE != params) {
		_print_shader_program_log(program);
		return false;
	}
	return true;
}


void ShaderProgram::_check_link_errors(GLuint program) {
	int params = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		log.gl_log_err("\nERROR: program, %i, didn't link\n", program);
		_print_shader_log(program);
	}
}


void ShaderProgram::load_shader(std::string name, GLenum type) {
	std::string shader = read_shader(std::string(name));
	const char* shader_cstr = shader.c_str();

	GLuint compiled_shader = glCreateShader(type);
	glShaderSource(compiled_shader, 1, &shader_cstr, NULL);
	glCompileShader(compiled_shader);

	_check_compile_errors(compiled_shader);

	switch (type)
	{
	case GL_VERTEX_SHADER:
		vertex_shader = {name, compiled_shader, shader, GL_VERTEX_SHADER };
		shaders[0] = vertex_shader;
		shader_count++;
		break;
	case GL_FRAGMENT_SHADER:
		fragment_shader = { name, compiled_shader, shader, GL_FRAGMENT_SHADER };
		shaders[1] = fragment_shader;
		shader_count++;
		break;
	case GL_GEOMETRY_SHADER:
		geometry_shader = { name, compiled_shader, shader, GL_GEOMETRY_SHADER };
		shaders[2] = geometry_shader;
		shader_count++;
		break;
	case GL_TESS_CONTROL_SHADER:
		tesscontrol_shader = { name, compiled_shader, shader, GL_TESS_CONTROL_SHADER };
		shaders[3] = tesscontrol_shader;
		shader_count++;
		break;
	case GL_TESS_EVALUATION_SHADER:
		tesseval_shader = { name, compiled_shader, shader, GL_TESS_EVALUATION_SHADER };
		shaders[4] = tesseval_shader;
		shader_count++;
		break;
	default:
		break;
	}
}

void ShaderProgram::attach_and_link() {
	
	if (vertex_shader.id > 0) {
		glAttachShader(program_index, vertex_shader.id);
	}
	if (geometry_shader.id > 0) {
		glAttachShader(program_index, geometry_shader.id);
	}
	if (tesscontrol_shader.id > 0) {
		glAttachShader(program_index, tesscontrol_shader.id);
	}
	if (tesseval_shader.id > 0) {
		glAttachShader(program_index, tesseval_shader.id);
	}
	if (fragment_shader.id > 0) {
		glAttachShader(program_index, fragment_shader.id);
	}
	
	glLinkProgram(program_index);
	// we no longer need them, they are linked in the program now
	if (fragment_shader.id > 0) {
		glDeleteShader(fragment_shader.id);
	}
	if (vertex_shader.id > 0) {
		glDeleteShader(vertex_shader.id);
	}
	if (geometry_shader.id > 0) {
		glDeleteShader(geometry_shader.id);
	}
	if (tesscontrol_shader.id > 0) {
		glDeleteShader(tesscontrol_shader.id);
	}
	if (tesseval_shader.id > 0) {
		glDeleteShader(tesseval_shader.id);
	}

	// check program link errors
	_check_link_errors(program_index);
	_print_all_shader_info(program_index);
	_is_valid(program_index);
}

void ShaderProgram::use()
{
	glUseProgram(program_index);
}

//DEVELOPMENT STUFF
/**
Reloads the shaders attached to the given program
*/
void ShaderProgram::reload_shaders() {
	/*int shaderCount = -1;
	glGetProgramiv(program_index, GL_ATTACHED_SHADERS, &shaderCount);
	//glGetShaderSource
	const int max_length = 64;
	GLuint shaders[max_length];
	int actual_length = 0;
	int size = 0;
	GLuint new_program = glCreateProgram();
	glGetAttachedShaders(program_index, max_length, &actual_length, shaders);
	for (GLuint i = 0; i < (GLuint)shaderCount; i++) {
		const int max_length = 2048;
		char source[max_length];
		int actual_length = 0;
		int size = 0;
		int param_type = -1;
		glGetShaderSource(shaders[i], max_length, &actual_length, source);
		glGetShaderiv(shaders[i], GL_SHADER_TYPE, &param_type);
		char shader_name[32];
		int pos = 0;
		for (int j = 0; j < actual_length; j++) {
			if (source[j] != '/') {
				shader_name[pos] = source[j];
				pos++;
			}
			if (source[j] == '\n') {
				shader_name[pos - 1] = '\0';
				break;
			}
		}
		load_shader(shader_name, param_type);
		//glAttachShader(new_program, reloaded_shader);
		//glDeleteShader(reloaded_shader);
	}*/
	GLuint new_program = glCreateProgram();
	shader new_shaders[5];
	int limit = shader_count;
	for (int i = 0; i < limit; i++) {
		new_shaders[i] = shaders[i];
	}
	shader_count = 0;
	for (int i = 0; i < limit; i++) {
		load_shader(new_shaders[i].name, new_shaders[i].type);
	}
	program_index = new_program;
	attach_and_link();
	//glLinkProgram(new_program);
	if (_is_valid(new_program)) {
		glDeleteProgram(program_index);
		program_index = new_program;
	}
	else {
		// Return the old program and notify the error
		log.gl_log_err("ERROR reloading shaders of program: %i", program_index);
		printf("ERROR reloading shaders of program: %i", program_index);
		_check_link_errors(new_program);
		program_index = program_index;
	}

}

ShaderProgram::~ShaderProgram()
{
}
