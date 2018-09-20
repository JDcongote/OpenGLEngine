#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 texture_coords;
layout(location = 3) in vec4 vertex_tangent;

uniform mat4 model;
uniform vec3 camera_position;
uniform vec3 light_position;

layout (std140) uniform cam_block {
  mat4 proj;
  mat4 view;
};

out vec2 tex_coords;
out vec3 light_pos, color, position_eye, normal_eye, normals, tangents, bi_tangents, view_dir_tangent, light_dir_tangent, light_position_eye;
out mat4 view_mat, model_mat;

void main(){
	
	//eye space variables
	position_eye = vec3(view * model * vec4(vertex_position, 1.0));	
	light_position_eye = vec3(view * vec4(light_position, 1.0));
	normal_eye = vec3(view * model * vec4(vertex_normals, 0.0));
	

	bi_tangents = cross(vertex_normals, vertex_tangent.xyz) * vertex_tangent.w; // to correct for hanedness
	
	//local space variables
	vec3 cam_pos_local = vec3 (inverse (model) * vec4 (camera_position, 1.0));
	vec3 light_dir_local = vec3 (inverse (model) * vec4 (light_position, 0.0));
	vec3 view_dir_local = normalize (cam_pos_local - vertex_position);

	//tangent space variables
	view_dir_tangent = vec3 (
		dot (vertex_tangent.xyz, view_dir_local),
		dot (bi_tangents, view_dir_local),
		dot (vertex_normals, view_dir_local)
	);
	light_dir_tangent = vec3 (
		dot (vertex_tangent.xyz, light_dir_local),
		dot (bi_tangents, light_dir_local),
		dot (vertex_normals, light_dir_local)
	 );	

	//pass through variables:
	view_mat = view;
	model_mat = model;
	light_pos = light_position;
	normals = vertex_normals;
	tangents = vertex_tangent.xyz;
	tex_coords = texture_coords;
	gl_Position  = proj * view * model * vec4(vertex_position, 1.0);	
}