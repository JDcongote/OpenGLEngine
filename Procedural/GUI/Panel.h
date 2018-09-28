#pragma once
#include "GUI.h"
#include "../Tools/FontLoader.h"
class Panel
{
public:
	Panel();
	Panel(int width, int height, float pos_x, float pos_y, WindowDimensions win);
	void render(GLuint model);
	Object panel_object;
	~Panel();
private:
	Tex2D panel_texture;
	WindowDimensions window_dimensions;
	unsigned char* buffer;
};

