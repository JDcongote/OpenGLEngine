//fragment.frag
#version 460
#define SPOTLIGHT 0

layout (binding = 0) uniform samplerCube tex_sampler0;


in vec3 light_pos, color, normals, tex_coords;
in mat4 view_mat, model_mat;

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 bright_color;


vec4 diffuse = texture (tex_sampler0, tex_coords);

void main() {
	vec3 final_color = diffuse.xyz;
    fragment_color = vec4(final_color, 1.0);
	float brightness = dot(fragment_color.rgb, vec3(0.5, 0.5, 0.5));
	if(brightness > 1.0)
        bright_color = vec4(fragment_color.rgb, 1.0);
    else
        bright_color = vec4(0.0, 0.0, 0.0, 1.0);
}