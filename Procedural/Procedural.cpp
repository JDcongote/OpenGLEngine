#include "Procedural.h"



Procedural::Procedural()
{
	log = Log();
	assert(log.restart_gl_log());

	camera = Camera(Vector3f(0.0f, 0.0f, 3.0f), 0.1f, 100.0f, 66.0f, 640.0f, 480.0f);
	raycaster = Raycaster(camera);
}

// Main Error Callback
void _glfw_error_callback(int error, const char* description) {
	printf("\nGLFW ERROR: code %i, message: %s\n", error, description);
}
// end callbacks


//refactor this when moving code to objects
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

int Procedural::initGL()
{
	log.gl_log("starting GLFW\n%s\n", glfwGetVersionString());
	// Register error callback
	glfwSetErrorCallback(_glfw_error_callback);

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
	window = glfwCreateWindow(640, 480, "GLengine", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: couldn't initialize window");
		glfwTerminate();
		return 1;
	}
	// needed for glfwGetUserPointer to work
	glfwSetWindowUserPointer(window, this);

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
	log.gl_log("Renderer: %s\n", renderer);
	log.gl_log("OpenGL version supported %s\n", version);

	

	// Define simple shaders	
	shader_program = ShaderProgram(log);
	shader_program.load_shader("vertex.vert", GL_VERTEX_SHADER);
	shader_program.load_shader("fragment.frag", GL_FRAGMENT_SHADER);
	shader_program.attach_and_link();

	//GLuint current_program = -1;

	//callbacks
	glfwSetMouseButtonCallback(window, _mouse_button_callback);
	glfwSetCursorPosCallback(window, _cursor_pos_callback);

	
}

int Procedural::render_loop() {
	//the following code will be refactored to OO	

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
	// LIGHT
	const float light_position_world[] = { 10.0f, 10.0f, 10.0f };
	Matrix4f mat4 = Matrix4f();
	//keep track of this variable, returns -1 if not found to be active
	GLuint model_matrix = glGetUniformLocation(shader_program.program_index, "model");
	GLuint view_matrix = glGetUniformLocation(shader_program.program_index, "view");
	GLuint proj_matrix = glGetUniformLocation(shader_program.program_index, "proj");
	GLuint light_position = glGetUniformLocation(shader_program.program_index, "light_position");

	glUniformMatrix4fv(view_matrix, 1, GL_FALSE, camera.get_view_matrix().get_as_array());
	glUniformMatrix4fv(proj_matrix, 1, GL_FALSE, camera.get_projection_matrix().get_as_array());
	while (!glfwWindowShouldClose(window)) {
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();

		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;


		// Clear bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		//current_program = shader_program;
		glUseProgram(shader_program.program_index);

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
			shader_program.reload_shaders();			
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


Procedural::~Procedural()
{
}

void Procedural::MouseClick(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera.should_move_camera = true;
		raycaster.raycast();
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		camera.should_move_camera = false;
	}
}

void Procedural::MouseMove(double xpos, double ypos)
{
	if (camera.should_move_camera && abs(prev_xpos - xpos) > 10) {
		camera.rotate_y((prev_xpos - xpos));
		camera.should_update = true;
	}
	if (camera.should_move_camera && abs(prev_ypos - ypos) > 10) {
		camera.rotate_x((prev_ypos - ypos));
		camera.should_update = true;
	}
	prev_xpos = xpos;
	prev_ypos = ypos;

	raycaster.update_coords_view(xpos, ypos);
}
