#include "Procedural.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Libs/stb_image.h"


Procedural::Procedural()
{
	log = Log();
	assert(log.restart_gl_log());

	camera = Camera(Vector3f(0.0f, 0.0f, 3.0f), 0.1f, 1000.0f, 66.0f, 1280, 720);
	raycaster = Raycaster(camera);
}

// Main Error Callback
void _glfw_error_callback(int error, const char* description) {
	printf("\nGLFW ERROR: code %i, message: %s\n", error, description);
}
// Main debug Callback
void APIENTRY _debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam) {

	char src_str[2048]; /* source */
	char type_str[2048]; /* type */
	char sev_str[2048]; /* severity */

	switch (source) {
	case 0x8246:
		strcpy(src_str, "API");
		break;
	case 0x8247:
		strcpy(src_str, "WINDOW_SYSTEM");
		break;
	case 0x8248:
		strcpy(src_str, "SHADER_COMPILER");
		break;
	case 0x8249:
		strcpy(src_str, "THIRD_PARTY");
		break;
	case 0x824A:
		strcpy(src_str, "APPLICATION");
		break;
	case 0x824B:
		strcpy(src_str, "OTHER");
		break;
	default:
		strcpy(src_str, "undefined");
		break;
	}
	switch (type) {
	case 0x824C:
		strcpy(type_str, "ERROR");
		break;
	case 0x824D:
		strcpy(type_str, "DEPRECATED_BEHAVIOR");
		break;
	case 0x824E:
		strcpy(type_str, "UNDEFINED_BEHAVIOR");
		break;
	case 0x824F:
		strcpy(type_str, "PORTABILITY");
		break;
	case 0x8250:
		strcpy(type_str, "PERFORMANCE");
		break;
	case 0x8251:
		strcpy(type_str, "OTHER");
		break;
	case 0x8268:
		strcpy(type_str, "MARKER");
		break;
	case 0x8269:
		strcpy(type_str, "PUSH_GROUP");
		break;
	case 0x826A:
		strcpy(type_str, "POP_GROUP");
		break;
	default:
		strcpy(type_str, "undefined");
		break;
	}
	switch (severity) {
	case 0x9146:
		strcpy(sev_str, "HIGH");
		break;
	case 0x9147:
		strcpy(sev_str, "MEDIUM");
		break;
	case 0x9148:
		strcpy(sev_str, "LOW");
		break;
	case 0x826B:
		strcpy(sev_str, "NOTIFICATION");
		break;
	default:
		strcpy(sev_str, "undefined");
		break;
	}

	fprintf(
		stderr,
		"source: %s type: %s id: %u severity: %s length: %i message: %s userParam: %i\n",
		src_str,
		type_str,
		id,
		sev_str,
		length,
		message,
		*(int*)userParam
	);
}
// end callbacks



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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);// DISABLE FOR RELEASE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	

	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	//const GLFWvidmode* modes = glfwGetVideoModes(monitors[1], &count);
	//const GLFWvidmode* current_mode = glfwGetVideoMode(monitors[1]);

	//GLFWwindow* window = glfwCreateWindow(current_mode->width, current_mode->height, "Hello Triangle", NULL, NULL);
	window = glfwCreateWindow(1280, 720, "GLengine", NULL, NULL);
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

	//switches
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	if (GLEW_KHR_debug) {
		int param = -1;
		printf("KHR_debug extension found\n");
		glDebugMessageCallback(_debug_callback, &param);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		printf("debug callback engaged\n");
	}
	else {
		printf("KHR_debug extension NOT found\n");
	}
	if (GLEW_ARB_uniform_buffer_object) {
		printf("GLEW_ARB_uniform_buffer_object = YES\n");
		GLint blocks = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &blocks);
		printf("GL_MAX_UNIFORM_BUFFER_BINDINGS = %i\n", blocks);
	}
	else {
		printf("GLEW_ARB_uniform_buffer_object = NO\n");
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	log.gl_log("\nRenderer: %s\n", renderer);
	log.gl_log("OpenGL version supported %s\n", version);

	// Define simple shaders		
	shader_program = ShaderProgram(log);
	shader_program.load_shader("default.vert", GL_VERTEX_SHADER);
	shader_program.load_shader("default.frag", GL_FRAGMENT_SHADER);
	shader_program.attach_and_link(shader_program.program_index);

	//callbacks
	glfwSetMouseButtonCallback(window, _mouse_button_callback);
	glfwSetCursorPosCallback(window, _cursor_pos_callback);
}

int Procedural::render_loop() {
	//the following code will be refactored to OO	
	// LIGHT
	const float light_position_world[] = {0.0f,10.0f,10.0f };
	//model matrix
	Matrix4f mat4 = Matrix4f();
	Matrix4f mat42 = Matrix4f();
	mat42.translate(2, 0, 0);

	Tex2D texture = Tex2D("test.png", GL_TEXTURE_2D);
	Tex2D texture2 = Tex2D("testing.png", GL_TEXTURE_2D);
	//Tex2D texture2 = Tex2D("testing.png", GL_TEXTURE_CUBE_MAP);

	//code for deferred rendering
	//unsigned char* buffer = (unsigned char*)malloc(g_fb_width * g_fb_height * 3);
	//glReadPixels(0, 0, g_fb_width, g_fb_height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

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
		/*LEFT SIDE*/
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		/*BACK SIDE*/
		-1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		/*BOTTOM SIDE*/
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
	};

	GLfloat texcoords[] = {
		0.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		/*Right*/
		1.0, 1.0,
		1.0, 0.0,
		0.0, 1.0,
		0.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,
		/*top*/
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,
		/*left*/
		1.0, 1.0,
		0.0, 0.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 1.0,
		1.0, 0.0,
		/*back*/
		1.0, 1.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
		/*bottom*/
		0.0, 1.0,
		1.0, 1.0,
		0.0, 0.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0
	};

	//std::vector<GLfloat> normals = calculate_normals(points, std::size(points));

	/*GLuint tex_coords = 0;
	glGenBuffers(1, &tex_coords);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);*/

	//UBO camera block
	GLuint camera_ubo;
	glGenBuffers(1, &camera_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 32 /*two matrices of 16 components*/, NULL, GL_DYNAMIC_DRAW);
	int block_id = 0;
	GLuint camera_ubo_index = glGetUniformBlockIndex(shader_program.program_index, "camera_ubo");//repeat this for every shader that´ll share this UBO
	glUniformBlockBinding(shader_program.program_index, camera_ubo_index, block_id);//repeat this for every shader that´ll share this UBO

	//load model
	GLuint vao = 0;
	int point_count;
	ObjectLoader::load_mesh("suzanne.obj", &vao, &point_count);
	GLuint box_vao = 0;
	int box_point_count;
	ObjectLoader::load_mesh("box.obj", &box_vao, &box_point_count);
	

	// Only define the vertex array object once, then draw
	
	/*glGenVertexArrays(1, &vao);
	/*glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coords);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);	*/

	//keep track of this variable, returns -1 if not found to be active
	GLuint model_matrix = glGetUniformLocation(shader_program.program_index, "model");
	GLuint view_matrix = glGetUniformLocation(shader_program.program_index, "view");
	GLuint proj_matrix = glGetUniformLocation(shader_program.program_index, "proj");
	GLuint light_position = glGetUniformLocation(shader_program.program_index, "light_position");
	GLuint camera_position = glGetUniformLocation(shader_program.program_index, "camera_position");

	glBindBufferBase(GL_UNIFORM_BUFFER, block_id, camera_ubo);
	float* camera_ubo_ptr = (float*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(float) * 32, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	memcpy(&camera_ubo_ptr[0], camera.get_projection_matrix().get_as_array(), sizeof(float) * 16);
	memcpy(&camera_ubo_ptr[16], camera.get_view_matrix().get_as_array(), sizeof(float) * 16);
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	while (!glfwWindowShouldClose(window)) {
		// Clear bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();

		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;


		
		//current_program = shader_program;
		glUseProgram(shader_program.program_index);

		// Expensive function!
		if (camera.should_update) {
			camera.should_update = false;

			glUniform3fv(camera_position, 1, camera.get_position().get_as_array());
			//glUniformMatrix4fv(view_matrix, 1, GL_FALSE, camera.get_view_matrix().get_as_array());
			//glUniformMatrix4fv(proj_matrix, 1, GL_FALSE, camera.get_projection_matrix().get_as_array());
			glUniform3fv(light_position, 1, light_position_world);
			memcpy(&camera_ubo_ptr[0], camera.get_projection_matrix().get_as_array(), sizeof(float) * 16);
			memcpy(&camera_ubo_ptr[16], camera.get_view_matrix().get_as_array(), sizeof(float) * 16);
		}

		

		
		glBindVertexArray(vao);
		texture.activate_texture(GL_TEXTURE0);
		texture2.activate_texture(GL_TEXTURE1);
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, mat4.get_as_array());
		glDrawArrays(GL_TRIANGLES, 0, point_count);
		//glUniformMatrix4fv(model_matrix, 1, GL_FALSE, mat42.get_as_array());
		//glDrawArrays(GL_TRIANGLES, 0, point_count); 

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
