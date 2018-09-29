#pragma once
#include "../CommonHeaders.h"
#include "../GUI/Panel.h"
#include "../Object/Object.h"
#include "../ShaderProgram/ShaderProgram.h"
class FrameBuffer
{
public:
	FrameBuffer(WindowDimensions window_dimensions, bool two_cbs, bool no_depth);

	void set_shader(ShaderProgram shader);
	void use_custom_gui_panel(Panel *custom_panel);
	void bind();
	GLuint model_mat;
	void unbind();

	void render(bool dont_clear);
	void render_ping_pong(FrameBuffer color, FrameBuffer other,int iterations);

	~FrameBuffer();
	GLuint index, depth_tex;
	GLuint *fb_tex;
	ShaderProgram framebuffer_shader;
	Object quad;
private:
	bool two_cbs = false;
	Panel *custom_panel = NULL;
	int color_attachments = 1;
	
	
};

