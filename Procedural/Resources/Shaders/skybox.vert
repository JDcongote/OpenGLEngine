//vertex.vert
#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 texture_coords;

uniform mat4 model_matrix;
uniform vec3 camera_position;
uniform vec3 light_position;

layout (std140) uniform cam_block {
  mat4 proj;
  mat4 view;
};

out vec3 tex_coords;
out vec3 light_pos, color, normals;
out mat4 view_mat, model_mat;

void main(){
	
	//eye space variables	
	view_mat = view;
	model_mat = model_matrix;
	light_pos = light_position;
	normals = -vertex_normals;
	tex_coords = vertex_position;
	//remove translation from view matrix
	view_mat[3] = vec4(0.0);
	view_mat[3][3] = 1.0;
	gl_Position  = proj * view_mat * model_matrix * vec4(vertex_position, 1.0);	
}