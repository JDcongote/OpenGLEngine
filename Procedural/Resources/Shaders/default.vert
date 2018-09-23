#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec3 texture_coords;
layout(location = 3) in vec4 vertex_tangent;

uniform mat4 model_matrix;
uniform vec3 camera_position;
uniform vec3 light_position;

layout (std140) uniform cam_block {
  mat4 proj;
  mat4 view;
};


out vec2 tex_coords;
out vec3 position_eye, normal_eye, light_position_eye, light_dir_tangent, view_dir_tangent, controlpoint_wor, normal_world, position_world;
out mat4 view_mat, model_mat, proj_mat;

void main(){
	
	//eye space variables
	position_eye = vec3(view * model_matrix * vec4(vertex_position, 1.0));	
	light_position_eye = vec3(view * model_matrix * vec4(light_position, 1.0));
	normal_eye = vec3(view * model_matrix * vec4(vertex_normals, 0.0));
	

	vec3 bi_tangents = cross(vertex_normals, vertex_tangent.xyz) * vertex_tangent.w; // to correct for hanedness
	
	//local space variables
	vec3 cam_pos_local = vec3 (inverse (model_matrix) * vec4 (camera_position, 1.0));
	vec3 light_dir_local = vec3 (inverse (model_matrix) * vec4 (-light_position, 0.0));
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
	position_world = (model_matrix * vec4(camera_position, 1.0)).xyz;
	normal_world = normalize(model_matrix * vec4(vertex_normals, 0.0)).xyz;
	controlpoint_wor = vertex_position;
	tex_coords = texture_coords.st;
	view_mat = view;
	model_mat = model_matrix;
	proj_mat = proj;	
	//gl_Position  = proj * view * model_matrix * vec4(vertex_position, 1.0);	
}