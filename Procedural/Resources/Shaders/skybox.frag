//fragment.frag
#version 460
#define SPOTLIGHT 0

layout (binding = 0) uniform samplerCube tex_sampler0;

in vec3 light_pos, color, normals, tex_coords;
in mat4 view_mat, model_mat;

out vec4 fragment_color;


vec4 diffuse = texture (tex_sampler0, tex_coords);

void main() {
	vec3 final_color = diffuse.xyz;
    fragment_color = vec4(final_color, 1.0);
}