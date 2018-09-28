#version 460

layout (binding = 0) uniform sampler2D tex_sampler0;
layout (binding = 1) uniform sampler2D tex_sampler1;

in vec2 tex_coords;

vec4 color_buffer0 = texture (tex_sampler0, tex_coords);
vec4 color_buffer1 = texture (tex_sampler1, tex_coords);

out vec4 frag_color;

const float gamma = 2.2;

void main(){
	
	frag_color = color_buffer0*5;
}