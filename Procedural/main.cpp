//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <assert.h>
#include <string>
#include <vector>
#include <iterator> 
#include <fstream>
#include "Math/Matrix/Matrix4f.h"
#include "Camera/Camera.h"
#include "Math/Raycast/Raycaster.h"

#define GL_LOG_FILE "gl.log"

bool restart_gl_log() {
	FILE* file = fopen(GL_LOG_FILE, "w");
	if (!file) {
		fprintf(stderr, "Can't open log file");
		return false;
	}

	time_t now = time(NULL);
	char* date = ctime(&now);

	fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
	fclose(file);
	return true;
}

bool gl_log(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "Error opening log file");
		return false;
	}
	va_start(argptr, message);

	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

bool gl_log_err(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

// Main Error Callback

void glfw_error_callback(int error, const char* description) {
	gl_log_err("\nGLFW ERROR: code %i, message: %s\n", error, description);
}

std::string  load_shader(std::string name) {
	std::string shader;

	name = "Shaders/" + name;
	std::string line;
	std::ifstream file(name);
	if (file.is_open()) {
		while (getline(file, line)) {
			shader += line + '\n';
		}
	}
	gl_log("\nSHADER: read shader: \n");
	gl_log(name.c_str());
	gl_log("\n");

	return shader;
}

void _print_shader_log(GLuint shader_index) {
	const int max_length = 2048;
	int actual_length = 0;
	char shader_log[max_length];
	glGetShaderInfoLog(shader_index, max_length, &actual_length, shader_log);
	printf("shader info log for GL index %u:\n%s\n", shader_index, shader_log);
}

void _print_shader_program_log(GLuint program) {
	const int max_length = 2048;
	int actual_length = 0;
	char shader_log[max_length];
	glGetProgramInfoLog(program, max_length, &actual_length, shader_log);
	printf("Program info log for GL index %u:\n%s\n", program, shader_log);
}

const char* GL_type_to_string(GLenum type) {
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
void _print_all_shader_info(GLuint program) {
	gl_log("\n\n--------------------\nshader program %i info:\n", program);
	int params = -1;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
	gl_log("GL_ATTACHED_SHADERS = %i\n", params);

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
	gl_log("GL_ACTIVE_ATTRIBUTES = %i\n", params);

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
				gl_log(long_name, "%s[%i]", name, j);

				int location = glGetAttribLocation(program, long_name);
				gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), long_name, location);
			}
		}
		else {
			int location = glGetAttribLocation(program, name);
			gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), name, location);
		}
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
	gl_log("GL_ACTIVE_UNIFORMS = %i\n", params);

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
				gl_log(long_name, "%s[%i]", name, j);

				int location = glGetUniformLocation(program, long_name);
				gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), long_name, location);
			}
		}
		else {
			int location = glGetUniformLocation(program, name);
			gl_log("  (%i) type: %s name: '%s' location: %i\n", i, GL_type_to_string(type), name, location);
		}
	}
}

/*
Disable for release, expensive function
*/
bool _is_valid(GLuint program) {
	glValidateProgram(program);
	int params = -1;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
	gl_log("program %i GL_VALIDATE_STATUS = %i\n", program, params);
	if (GL_TRUE != params) {
		_print_shader_program_log(program);
		return false;
	}
	return true;
}

void check_compile_errors(GLuint shader_index) {
	int params = -1;
	glGetShaderiv(shader_index, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		gl_log_err("\nERROR: Shader, %i, didn't compile\n", shader_index);
		fprintf(stderr, "\nERROR: Shader, %i, didn't compile\n", shader_index);
		_print_shader_log(shader_index);
	}
}

void check_link_errors(GLuint program) {
	int params = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		gl_log_err("\nERROR: program, %i, didn't link\n", program);
		fprintf(stderr, "\nERROR: program, %i, didn't link\n", program);
		_print_shader_log(program);
	}
}

GLuint create_shader(const char* name, GLenum type) {

	std::string shader = load_shader(std::string(name));
	const char* shader_cstr = shader.c_str();

	GLuint compiled_shader = glCreateShader(type);
	glShaderSource(compiled_shader, 1, &shader_cstr, NULL);
	glCompileShader(compiled_shader);

	check_compile_errors(compiled_shader);

	return compiled_shader;
}

/**
Reloads the shaders attached to the given program
*/
GLuint reload_shaders(GLuint program) {
	int shaderCount = -1;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaderCount);
	//glGetShaderSource
	const int max_length = 64;
	GLuint shaders[max_length];
	int actual_length = 0;
	int size = 0;
	GLuint new_program = glCreateProgram();
	glGetAttachedShaders(program, max_length, &actual_length, shaders);
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
		GLuint reloaded_shader = create_shader(shader_name, param_type);
		glAttachShader(new_program, reloaded_shader);
		glDeleteShader(reloaded_shader);
	}
	glLinkProgram(new_program);
	if (_is_valid(new_program)) {
		glDeleteProgram(program);
		return new_program;
	}
	else {
		// Return the old program and notify the error
		gl_log_err("ERROR reloading shaders of program: %i", program);
		printf("ERROR reloading shaders of program: %i", program);
		check_link_errors(new_program);
		return program;
	}

}
bool should_move_camera = false;
double prev_xpos, prev_ypos;
Camera camera = Camera(Vector3f(0.0f, 0.0f, 3.0f), 0.1f, 100.0f, 66.0f, 640.0f, 480.0f);
Raycaster raycaster = Raycaster(camera);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		should_move_camera = true;
		raycaster.raycast();
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		should_move_camera = false;
	}
}
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (should_move_camera && abs(prev_xpos - xpos) > 10) {
		camera.rotate_y((prev_xpos - xpos));
		camera.should_update = true;
	}
	if (should_move_camera && abs(prev_ypos - ypos) > 10) {
		camera.rotate_x((prev_ypos - ypos));
		camera.should_update = true;
	}
	prev_xpos = xpos;
	prev_ypos = ypos;

	raycaster.update_coords_view(xpos, ypos);
}

std::vector<GLfloat> calculate_normals(const float* points, const int size) {
	std::vector<Vector3f> vec_array(size / 3);
	std::vector<Vector3f> normals;
	std::vector<GLfloat> final_normals(size);

	for (int i = 0; i < size / 3; i++) {
		Vector3f vec = Vector3f(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]);
		vec_array[i] = vec;
	}
	//not the best way but its only for development purposes
	int limit = (size / 3) - 2; //loop run once per triangle, two triangles per face
	for (int i = 0; i < limit; i += 3)
	{
		Vector3f p0 = Vector3f(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]);
		Vector3f p1 = Vector3f(points[i * 3 + 3], points[i * 3 + 4], points[i * 3 + 5]);
		Vector3f p2 = Vector3f(points[i * 3 + 6], points[i * 3 + 7], points[i * 3 + 8]);

		Vector3f edge_1 = (p1 - p2).normalize();
		Vector3f edge_2 = (p2 - p0).normalize();
		Vector3f normal = edge_1 * edge_2;
		normals.push_back(normal);
	}
	int j = 0;
	limit = 9;
	for (int i = 0; i < normals.size(); i++) {
		for (j; j < limit; j += 3) {
			final_normals[j] = normals[i].x * -1;
			final_normals[j + 1] = normals[i].y * -1;
			final_normals[j + 2] = normals[i].z * -1;
		}
		limit += 9;
	}
	return final_normals;
}

int main(void)
{
	assert(restart_gl_log());

	gl_log("starting GLFW\n%s\n", glfwGetVersionString());
	// Register error callback
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: Can't initialize OpenGL");
		return 1;
	}


	// Set hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//switches
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	//const GLFWvidmode* modes = glfwGetVideoModes(monitors[1], &count);
	//const GLFWvidmode* current_mode = glfwGetVideoMode(monitors[1]);

	//GLFWwindow* window = glfwCreateWindow(current_mode->width, current_mode->height, "Hello Triangle", NULL, NULL);
	GLFWwindow* window = glfwCreateWindow(640, 480, "GLengine", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: couldn't initialize window");
		glfwTerminate();
		return 1;
	}

	// Our camera



	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		fprintf(stderr, "OpenGL Error: %i", error);
	}

	GLenum glewinit = glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	gl_log("Renderer: %s\n", renderer);
	gl_log("OpenGL version supported %s\n", version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Matrix4f mat4 = Matrix4f();


	GLfloat points[] = {
		/*FRONT SIDE*/
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		/*RIGHT SIDE*/
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		/*TOP SIDE*/
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		/*SIDE1*/
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		/*SIDE 2*/
		-1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		/*SIDE 3*/
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,		
	};

	std::vector<GLfloat> normals = calculate_normals(points, std::size(points));

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

	GLuint normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_DYNAMIC_DRAW);


	// Only define the vertex array object once, then draw
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Define simple shaders

	GLuint v_shader_gl = create_shader("vertex.vert", GL_VERTEX_SHADER);
	GLuint f_shader_gl = create_shader("fragment.frag", GL_FRAGMENT_SHADER);

	// Keep track of this variable
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, f_shader_gl);
	glAttachShader(shader_program, v_shader_gl);
	glLinkProgram(shader_program);
	// we no longer need them, they are linked in the program now
	glDeleteShader(f_shader_gl);
	glDeleteShader(v_shader_gl);

	// check program link errors
	check_link_errors(shader_program);

	//_print_all_shader_info(shader_program);
	_is_valid(shader_program);

	//keep track of this variable, returns -1 if not found to be active
	GLuint model_matrix = glGetUniformLocation(shader_program, "model");
	GLuint view_matrix = glGetUniformLocation(shader_program, "view");
	GLuint proj_matrix = glGetUniformLocation(shader_program, "proj");
	GLuint light_position = glGetUniformLocation(shader_program, "light_position");

	glUniformMatrix4fv(view_matrix, 1, GL_FALSE, camera.get_view_matrix().get_as_array());
	glUniformMatrix4fv(proj_matrix, 1, GL_FALSE, camera.get_projection_matrix().get_as_array());

	GLuint current_program = -1;

	//callbacks
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	// LIGHT
	const float light_position_world[] = { 10.0f, 10.0f, 10.0f };

	while (!glfwWindowShouldClose(window)) {
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();

		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;


		// Clear bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		current_program = shader_program;
		glUseProgram(current_program);

		// Expensive function!
		if (camera.should_update) {
			camera.should_update = false;
			
			glUniformMatrix4fv(model_matrix, 1, GL_FALSE, mat4.get_as_array());
			glUniformMatrix4fv(view_matrix, 1, GL_FALSE, camera.get_view_matrix().get_as_array());
			glUniformMatrix4fv(proj_matrix, 1, GL_FALSE, camera.get_projection_matrix().get_as_array());
			glUniform3fv(light_position, 1, light_position_world);
		}

		glBindVertexArray(vao);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		glfwSwapBuffers(window);

		// Close with escape
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

		if (glfwGetKey(window, GLFW_KEY_D)) {
			camera.translate(TRANSLATE_RIGHT, -(elapsed_seconds));
			camera.should_update = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			camera.translate(TRANSLATE_LEFT, elapsed_seconds);
			camera.should_update = true;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			camera.translate(TRANSLATE_BACKWARDS, -(elapsed_seconds));
			camera.should_update = true;
		}
		if (glfwGetKey(window, GLFW_KEY_W)) {
			camera.translate(TRANSLATE_FORWARD, elapsed_seconds);
			camera.should_update = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			camera.rotate_y(-(elapsed_seconds));
			camera.should_update = true;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			camera.rotate_y(elapsed_seconds);
			camera.should_update = true;
		}
		glfwPollEvents();
		camera.update();

		// Only for development!
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R)) {
			current_program = reload_shaders(current_program);
			shader_program = current_program;
			camera.should_update = true;
		}
		Vector3f ray_world = raycaster.get_ray_world();
		float points[] = {
			ray_world.x, ray_world.y, ray_world.z,
			ray_world.x + 0.2f, ray_world.y + 0.2f, ray_world.z + 100.0f,
		};
		//end dev
	}

	glfwTerminate();
	return 0;
}
