#pragma once
//include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

#include <assert.h>
#include <vector>
#include <iterator> 

#include "Log/Log.h"
#include "Math/Matrix/Matrix4f.h"
#include "Camera/Camera.h"
#include "ShaderProgram/ShaderProgram.h";
#include "Math/Raycast/Raycaster.h"
class Procedural
{
public:
	Log log;
	Procedural();

	void glfw_error_callback(int error, const char * description);

	int initGL();

	int render_loop();

	~Procedural();
private:	
	inline static auto _mouse_button_callback(GLFWwindow * gl_window, int button, int action, int mods) -> void {
		Procedural *window = static_cast<Procedural*>(glfwGetWindowUserPointer(gl_window));
		window->MouseClick(button, action, mods);
	}
	inline static auto _cursor_pos_callback(GLFWwindow * gl_window, double xpos, double ypos) -> void {
		Procedural *window = static_cast<Procedural*>(glfwGetWindowUserPointer(gl_window));
		window->MouseMove(xpos, ypos);
	};

	void MouseClick(int button, int action, int mods);
	void MouseMove(double xpos, double ypo);
	double prev_xpos, prev_ypos;

	//temporary
	ShaderProgram shader_program;

	GLFWwindow* window;
	Camera camera;
	Raycaster raycaster;
};

