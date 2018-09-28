#include "Procedural.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Libs/stb_image.h"

Development dev_vars = Development();
Procedural::Procedural()
{

	log = Log();
	assert(log.restart_gl_log());

	window_dimensions = { 1280,720 };

	camera = Camera(Vector3f(0.0f, 0.0f, 3.0f), 0.01f, 1000.0f, 66.0f, window_dimensions);
	raycaster = Raycaster(camera);
}

// Main Error Callback
void _glfw_error_callback(int error, const char* description) {
	printf("\nGLFW ERROR: code %i, message: %s\n", error, description);
}
// Main debug Callback
GLchar lastmessage[2048];
void APIENTRY _debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam) {
	if (strcmp(lastmessage, message) != 0) {
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
		strcpy(lastmessage, message);
	}

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	int max_patch_vertices = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &max_patch_vertices);
	printf("Max supported patch vertices %i\n", max_patch_vertices);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	log.gl_log("\nRenderer: %s\n", renderer);
	log.gl_log("OpenGL version supported %s\n", version);



	//callbacks
	glfwSetMouseButtonCallback(window, _mouse_button_callback);
	glfwSetCursorPosCallback(window, _cursor_pos_callback);
}

int Procedural::render_loop() {

	// Define simple shaders		
	ShaderProgram default_shader = ShaderProgram(log);
	default_shader.load_shader("default.vert", GL_VERTEX_SHADER);
	default_shader.load_shader("default.frag", GL_FRAGMENT_SHADER);
	default_shader.load_shader("default.tesc", GL_TESS_CONTROL_SHADER);
	default_shader.load_shader("default.tese", GL_TESS_EVALUATION_SHADER);
	default_shader.attach_and_link(default_shader.program_index);

	ShaderProgram gui_shader = ShaderProgram(log);
	gui_shader.load_shader("gui.vert", GL_VERTEX_SHADER);
	gui_shader.load_shader("gui.frag", GL_FRAGMENT_SHADER);
	gui_shader.attach_and_link(gui_shader.program_index);

	ShaderProgram skybox_shader = ShaderProgram(log);
	skybox_shader.load_shader("skybox.vert", GL_VERTEX_SHADER);
	skybox_shader.load_shader("skybox.frag", GL_FRAGMENT_SHADER);
	skybox_shader.attach_and_link(skybox_shader.program_index);

	//the following code will be refactored to OO	
	// LIGHT
	const float light_position_world[] = { 0.0f,5.0f,5.0f };

	//Tex2D texture2 = Tex2D("testing.png", GL_TEXTURE_CUBE_MAP);



	//UBO camera block
	camera.set_ubo();
	GLuint camera_ubo_index = glGetUniformBlockIndex(default_shader.program_index, "cam_block");//repeat this for every shader that´ll share this UBO
	glUniformBlockBinding(default_shader.program_index, camera_ubo_index, camera.ubo_blockid);//repeat this for every shader that´ll share this UBO

	// gui	
	Panel test_panel = Panel(512, 512, 0, 0, window_dimensions);
	//load models  
	Tex2D cubemap = Tex2D("cube", GL_TEXTURE_CUBE_MAP);
	Object dinosaur = Object("box.obj", true, 5, Tex2D("diffuse.png", GL_TEXTURE_2D), Tex2D("specular.png", GL_TEXTURE_2D), Tex2D("normal.png", GL_TEXTURE_2D),
		cubemap, Tex2D("height.png", GL_TEXTURE_2D));
	Object skybox = Object("box.obj", false, 1, cubemap);
	skybox.model_matrix.scale(100.0f, 100.0f, 100.0f);

	//keep track of this variable, returns -1 if not found to be active
	//GLuint pn_flag = glGetUniformLocation(default_shader.program_index, "USE_PN");
	//glUseShader(default_shader.program_index);
	//GLint use_pn = 1;
	//glUniform1iv(pn_flag, 1, &use_pn);
	GLuint model_matrix = glGetUniformLocation(default_shader.program_index, "model_matrix");
	GLuint light_position = glGetUniformLocation(default_shader.program_index, "light_position");
	GLuint camera_position = glGetUniformLocation(default_shader.program_index, "camera_position");

	
	FrameBuffer render_to_texture = FrameBuffer(window_dimensions, false, true);
	//render_to_texture.use_custom_gui_panel(&test_panel);
	FrameBuffer ping_pong_framebuffer1 = FrameBuffer(window_dimensions, true, false);
	FrameBuffer ping_pong_framebuffer2 = FrameBuffer(window_dimensions, true, true);

	//ping pong shader
	ShaderProgram pingpong_shader = ShaderProgram();
	pingpong_shader.load_shader("post.vert", GL_VERTEX_SHADER);
	pingpong_shader.load_shader("Postprocess/bloom.frag", GL_FRAGMENT_SHADER);
	pingpong_shader.attach_and_link(pingpong_shader.program_index);
	ping_pong_framebuffer1.set_shader(pingpong_shader);

	Panel ping_pong_panel_1 = Panel(256, 256, 0, 0, window_dimensions);
	Panel ping_pong_panel_2 = Panel(256, 256, 256, 0, window_dimensions);
	ping_pong_panel_1.panel_object.model_matrix.translate_z(-0.01f);
	ping_pong_panel_2.panel_object.model_matrix.translate_z(-0.01f);
	ping_pong_framebuffer1.use_custom_gui_panel(&ping_pong_panel_1);
	ping_pong_framebuffer2.use_custom_gui_panel(&ping_pong_panel_2);

	// MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		//framebuffers
		if (dev_vars.use_framebuffers) {
			render_to_texture.bind();
		}

		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		

		//render skybox
		glDepthMask(GL_FALSE);
		glUseProgram(skybox_shader.program_index);

		glCullFace(GL_FRONT);
		skybox.render(glGetUniformLocation(skybox_shader.program_index, "model_matrix"));
		glDepthMask(GL_TRUE);
		glCullFace(GL_BACK);
		//current_program = default_shader;
		glUseProgram(default_shader.program_index);

		// Expensive function!
		if (camera.should_update) {
			glUniform3fv(camera_position, 1, camera.get_position().get_as_array());
			glUniform3fv(light_position, 1, light_position_world);
			camera.update();
			camera.should_update = false;
		}
		if (dev_vars.use_wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		dinosaur.render(model_matrix);

		//ping_pong_framebuffer1.bind();		
		//dinosaur.render(model_matrix);
		//ping_pong_framebuffer1.unbind();

		if (dev_vars.use_wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}		

		if (dev_vars.use_framebuffers) {
			render_to_texture.unbind();

			ping_pong_framebuffer1.bind();
			render_to_texture.render(true);
			ping_pong_framebuffer1.unbind();

			ping_pong_framebuffer1.render(false);
			//ping_pong_framebuffer2.render(true);
		}
		//render gui		
		glEnable(GL_BLEND);
		glUseProgram(gui_shader.program_index);
		//test_panel.render(glGetUniformLocation(gui_shader.program_index, "model_matrix")); // dont query uniforms always... fix this
		glDisable(GL_BLEND);

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

		// Only for development!
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R)) {
			default_shader.reload_shaders();
			camera.should_update = true;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_P)) {
			dev_vars.use_wireframe = !dev_vars.use_wireframe;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_B)) {
			dev_vars.use_framebuffers = !dev_vars.use_framebuffers;
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
