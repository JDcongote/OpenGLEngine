#pragma once
#include "../CommonHeaders.h"
#include "../GUI/Panel.h"
#include "../Object/Object.h"
#include "../ShaderProgram/ShaderProgram.h"
class FrameBuffer
{
public:
	FrameBuffer(WindowDimensions window_dimensions, bool two_cbs, bool create_shader);

	void set_shader(ShaderProgram shader);
	void use_custom_gui_panel(Panel *custom_panel);
	void bind();
	GLuint model_mat;
	void unbind();

	void render(bool dont_clear);

	~FrameBuffer();
	GLuint index, depth_tex;
	GLuint *fb_tex;
private:
	bool two_cbs = false;
	Panel *custom_panel = NULL;
	int color_attachments = 1;
	ShaderProgram framebuffer_shader;
	Object quad;	
};

