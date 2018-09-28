#version 460

layout (binding = 0) uniform sampler2D tex_sampler0;

layout (location = 1) out vec4 bloom;
layout (location = 0) out vec4 frag_color;

in vec2 tex_coords;

vec4 color_buffer0 = texture (tex_sampler0, tex_coords);

const float gamma = 2.2;

void main(){
	
	float brightness = dot(color_buffer0.rgb, vec3(0.8, 0.8, 0.8));
	color_buffer0 = pow(color_buffer0, vec4(1.0 / gamma));
	frag_color = color_buffer0;
	vec4 bloom;
	if(brightness > 1.0)
        bloom = vec4(frag_color.rgb, 1.0);
    else
        bloom = vec4(0.0, 0.0, 0.0, 1.0);
	bloom = vec4(1.0,0.0,1.0,1.0);
}