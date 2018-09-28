#include "FrameBuffer.h"




FrameBuffer::FrameBuffer(WindowDimensions window_dimensions, bool two_cbs, bool create_shader)
{

	this->two_cbs = two_cbs;
	fb_tex = new GLuint[1];
	if (two_cbs) {
		fb_tex = new GLuint[2];
		glGenTextures(2, fb_tex);
		color_attachments = 2;
	}
	else {
		glGenTextures(1, fb_tex);
	}
	glGenFramebuffers(1, &index);	
	for (int i = 0; i < color_attachments; i++)
	{		
		glBindTexture(GL_TEXTURE_2D, fb_tex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, window_dimensions.width, window_dimensions.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, index);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fb_tex[i], 0);
	}
	
	//depth texture
	glGenTextures(1, &depth_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depth_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_dimensions.width, window_dimensions.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attach depth texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);
	if (two_cbs) {
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
	}
	else {
		unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//create vao or use the already built
	if (!generic_quad_built) {
		glGenBuffers(1, &generic_quad_vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, generic_quad_vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		glGenVertexArrays(1, &generic_quad_vao);
		glBindVertexArray(generic_quad_vao);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		_build_generic_quad();
	}	
	if (create_shader) {
		framebuffer_shader = ShaderProgram();
		framebuffer_shader.load_shader("post.vert", GL_VERTEX_SHADER);
		framebuffer_shader.load_shader("post.frag", GL_FRAGMENT_SHADER);
		framebuffer_shader.attach_and_link(framebuffer_shader.program_index);
	}	
}


void FrameBuffer::set_shader(ShaderProgram shader)
{
	framebuffer_shader = shader;
}

void FrameBuffer::use_custom_gui_panel(Panel* custom_panel)
{	
	this->custom_panel = custom_panel;
	model_mat = glGetUniformLocation(framebuffer_shader.program_index, "model_matrix");
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, index);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::render(bool dont_clear ) {
	if (!dont_clear) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}	
	glUseProgram(framebuffer_shader.program_index);

	for (int i = 0; i < color_attachments; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, fb_tex[i]);
	}
	if (custom_panel) {
		glUniformMatrix4fv(model_mat, 1, GL_FALSE, custom_panel->panel_object.model_matrix.get_as_array());
		glBindVertexArray(custom_panel->panel_object.get_vao());
	}
	else {
		glBindVertexArray(generic_quad_vao);
	}	
	glDrawArrays(GL_TRIANGLES,0, 6);
}

FrameBuffer::~FrameBuffer()
{
}
