#version 460

layout (binding = 0) uniform sampler2D tex_sampler0;
layout (binding = 1) uniform sampler2D tex_sampler1;

in vec2 tex_coords;
out vec4 frag_color;

vec4 diffuse = texture (tex_sampler0, tex_coords);
vec4 depth = texture (tex_sampler1, tex_coords);

void main(){
	diffuse.a = 1.0;
	frag_color = vec4(1.0,1.0,0.0,1.0);	
	frag_color.rgb = pow (frag_color.rgb, vec3 (1.0 / 2.5, 1.0 / 2.5, 1.0 / 2.5));
}