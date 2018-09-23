#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec3 texture_coords;
layout(location = 3) in vec4 vertex_tangent;

layout (std140) uniform cam_block {
  mat4 proj;
  mat4 view;
};

uniform mat4 model_matrix;
uniform vec3 camera_position, light_position;

in vec3 vp_loc;
out vec3 controlpoint_wor;
out vec2 tex_coords;
out mat4 view_mat, model_mat, proj_mat;

void main() {
	controlpoint_wor = vp_loc;
	tex_coords = texture_coords.st;

	view_mat = view;
	model_mat = model_matrix;
	proj_mat = proj;
}
