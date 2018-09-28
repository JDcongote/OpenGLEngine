#include "Panel.h"


float clamp(float x, float min, float max) {
	if (x < min) {
		x = min;
	}
	else if (x > max) {
		x = max;
	}
	return x;
}

Panel::Panel()
{
}

Panel::Panel(int width, int height, float pos_x, float pos_y, WindowDimensions win)
{
	//const int buffer_size = win.width * win.height * 4;
	//buffer = (unsigned char*)new char[buffer_size];
	//window_dimensions = win;
	//panel_texture = Tex2D(buffer, GL_TEXTURE_2D, win.width, win.height);
	//load_font("OpenSans");
	//create vao
	if (!generic_quad_built) {
		glGenBuffers(1, &generic_quad_vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, generic_quad_vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		glGenVertexArrays(1, &generic_quad_vao);
		glBindVertexArray(generic_quad_vao);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		_build_generic_quad();
		panel_object = Object(&generic_quad_vao);
	}

	if (pos_x + width > win.width) {
		pos_x = win.width - width;
	}
	if (pos_y + height > win.height) {
		pos_y = win.height - height;
	}

	float position_x = (1 * pos_x) / win.width;
	float position_y = (1 * pos_y) / win.height;
	

	panel_object.model_matrix.set_scale((float)width / win.width, (float)height / win.height, 0.0f);
	panel_object.model_matrix.set_position(position_x, position_y, 0.0);
}

void Panel::render(GLuint model) {
	//glReadPixels(0, 0, window_dimensions.width, window_dimensions.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	//anel_texture.activate_texture(GL_TEXTURE0);
	panel_object.render(model);

}

Panel::~Panel()
{
}
