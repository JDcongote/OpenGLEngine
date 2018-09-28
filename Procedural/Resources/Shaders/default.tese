#version 460
layout (binding = 4) uniform sampler2D height;

// triangles, quads, or isolines
layout (triangles, equal_spacing, ccw) in;
in vec3 evaluationpoint_wor[];
patch in mat4 viewmat, modelmat, projmat;

in vec2 texcoords[];
in vec3 normaleye[], positioneye[], lightpositioneye[], lightdirtangent[], viewdirtangent[], normalworld[];

out vec3 position_eye, normal_eye, light_position_eye, light_dir_tangent, view_dir_tangent;
out vec2 tex_coords;
out mat4 view_mat, model_mat, proj_mat;

// gl_TessCoord is location within the patch
// (barycentric for triangles, UV for quads)

float tess_multiplier = 0.02;

vec3 interpolate_tesscoord_3D(vec3 v0, vec3 v1, vec3 v2){
	 return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec2 interpolate_tesscoord_2D(vec2 v0, vec2 v1, vec2 v2){
	 return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

void main () {

	vec3 pos = interpolate_tesscoord_3D(evaluationpoint_wor[0], evaluationpoint_wor[1], evaluationpoint_wor[2]);

	//pasthorugh
	tex_coords = interpolate_tesscoord_2D(texcoords[0], texcoords[1], texcoords[2]);
	position_eye = interpolate_tesscoord_3D(positioneye[0], positioneye[1], positioneye[2]);
	normal_eye = interpolate_tesscoord_3D(normaleye[0], normaleye[1], normaleye[2]);
	vec3 normal_world = interpolate_tesscoord_3D(normalworld[0], normalworld[1], normalworld[2]);

	light_position_eye = interpolate_tesscoord_3D(lightpositioneye[0], lightpositioneye[1], lightpositioneye[2]);
	light_dir_tangent = interpolate_tesscoord_3D(lightdirtangent[0], lightdirtangent[1], lightdirtangent[2]);
	view_dir_tangent = interpolate_tesscoord_3D(viewdirtangent[0], viewdirtangent[1], viewdirtangent[2]);

	view_mat = viewmat;
	model_mat = modelmat;
	proj_mat = projmat;

	float displacement = texture(height, tex_coords.xy).x;
	pos += normal_world * displacement * tess_multiplier;
	gl_Position  = projmat * viewmat * modelmat * vec4(pos, 1.0);	
}