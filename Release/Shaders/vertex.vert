//vertex.vert
#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 texture_coords;

uniform mat4 model;
uniform vec3 light_position;
layout (std140) uniform cam_block {
  mat4 proj;
  mat4 view;
};

out vec2 tex_coords;
out vec3 light_pos, color, position_eye, normal_eye, normals;
out mat4 view_mat;

void main(){
	light_pos = light_position;
	position_eye = vec3(view * model * vec4(vertex_position, 1.0));
	normal_eye = vec3(view * model * vec4(vertex_normals, 0.0));
	normals = vertex_normals;
	view_mat = view;
	tex_coords = texture_coords;
	gl_Position  = proj * view * model * vec4(vertex_position, 1.0);	
}