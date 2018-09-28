#version 460

uniform int USE_PN;

// number of CPs in patch
layout (vertices = 3) out;


in vec3 controlpoint_wor[], position_eye[], normal_eye[], light_position_eye[], light_dir_tangent[], view_dir_tangent[], normal_world[], position_world[];
in vec2 tex_coords[];
in mat4 view_mat[], model_mat[], proj_mat[];

// to evluation shader. will be used to guide positioning of generated points
out vec3 evaluationpoint_wor[];
patch out mat4 viewmat, modelmat, projmat;

out vec2 texcoords[];
out vec3 normaleye[], positioneye[], lightpositioneye[], lightdirtangent[], viewdirtangent[], normalworld[];

uniform float tess_fac_min = 1.0;
uniform float tess_fac_max = 100.0; 

float GetTessLevel(float Distance0, float Distance1)
{
	float ratio = 3.0; // the higher this value the farther away higher detail is seen.
	
	float l = (Distance0 + Distance1) / 3.0;
	float mixcontrol = mix(1.0, 0.0, (l/ratio)); 
	float tess_factor = mix(tess_fac_min, tess_fac_max, mixcontrol);

	return clamp(tess_factor, tess_fac_min, tess_fac_max);
} 

void main () {
	evaluationpoint_wor[gl_InvocationID] = controlpoint_wor[gl_InvocationID];

	float EyeToVertexDistance0 = distance(position_world[gl_InvocationID], evaluationpoint_wor[0]);
    float EyeToVertexDistance1 = distance(position_world[gl_InvocationID], evaluationpoint_wor[1]);
    float EyeToVertexDistance2 = distance(position_world[gl_InvocationID], evaluationpoint_wor[2]);

	// Calculate the tessellation levels
	gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];

	normalworld[gl_InvocationID] = normal_world[gl_InvocationID];
	texcoords[gl_InvocationID] = tex_coords[gl_InvocationID];
	positioneye[gl_InvocationID] = position_eye[gl_InvocationID];
	normaleye[gl_InvocationID] = normal_eye[gl_InvocationID];

	//test var
	//float l = length(-position_eye[gl_InvocationID]);
	//l = mix(1.0, 0.0, (l/10.0));
	//normaleye[gl_InvocationID] = vec3(l, l, l);

	lightpositioneye[gl_InvocationID] = light_position_eye[gl_InvocationID];
	lightdirtangent[gl_InvocationID] = light_dir_tangent[gl_InvocationID];
	viewdirtangent[gl_InvocationID] = view_dir_tangent[gl_InvocationID];

	viewmat = view_mat[gl_InvocationID];
	modelmat = model_mat[gl_InvocationID];
	projmat = proj_mat[gl_InvocationID];
}