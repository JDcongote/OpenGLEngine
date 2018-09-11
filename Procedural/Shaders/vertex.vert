//fragment.frag
#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;

uniform mat4 proj,view,model;
uniform vec3 light_position;

out vec3 light_pos, color, position_eye, normal_eye;
out mat4 view_mat;

void main(){
	light_pos = light_position;
	position_eye = vec3(view * model * vec4(vertex_position, 1.0));
	normal_eye = vec3(view * model * vec4(vertex_normals, 0.0));
	view_mat = view;
	gl_Position  = proj * view * model * vec4(vertex_position, 1.0);	
}