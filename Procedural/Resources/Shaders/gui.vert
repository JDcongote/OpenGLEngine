#version 460

layout(location = 0) in vec2 vertex_position;

uniform mat4 model_matrix;

out vec2 tex_coords;

void main(){
	tex_coords = (vertex_position + 1.0) * 0.5;

	float pos_x = mix(-1.0, 1.0, model_matrix[3][0]);
	float pos_y = mix(-1.0, 1.0, model_matrix[3][1]);

	mat4 model = model_matrix;
	model[3][0] = pos_x;
	model[3][1] = pos_y;
	
	gl_Position = vec4(vertex_position.xy, 0.0,1.0);
	gl_Position.xy += 1.0;
	gl_Position = model * gl_Position;
	gl_Position.z = -0.01;
}