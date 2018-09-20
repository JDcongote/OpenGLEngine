//fragment.frag
#version 460
#define SPOTLIGHT 0

layout (binding = 0) uniform sampler2D tex_sampler0;
layout (binding = 1) uniform sampler2D tex_sampler1;

//in vec3 color;
in vec2 tex_coords;
in vec3 light_pos, color, position_eye, normals, tangents, bi_tangents, normal_eye, light_position_eye, light_dir_tangent, view_dir_tangent;
in mat4 view_mat ,model_mat;

out vec4 fragment_color;

// Phong components
float I,Id,Is,Ia;
vec3 Ls = vec3(1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3(0.7, 0.7, 0.7); // dull white diffuse light colour
vec3 La = vec3(0.2, 0.2, 0.2); // grey ambient colour

// these should be passed per object thorugh the vertex shader
// surface reflectance
vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
vec3 Kd = vec3 (0.5, 0.5, 0.5); // orange diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 20.0; // specular 'power'


vec4 diffuse = texture (tex_sampler0, tex_coords);
vec4 normal_map = texture (tex_sampler1, tex_coords);

void main() {
	//Kd = diffuse.rgb;
	//Ks = clamp(vec4(pow(diffuse.r, 10.0), pow(diffuse.g, 10.0), pow(diffuse.b, 10.0), diffuse.a), 0.0, 1.0).rgb;

	//eye space variables	
	vec3 distance_to_light_eye  = light_position_eye - position_eye;
	vec3 direction_to_light_eye = normalize(distance_to_light_eye);

	//tangent space variables
	vec3 normal_tangent = normalize (normal_map.xyz * 2.0 - 1.0);
	vec3 direction_to_light_tangent  = normalize(-light_dir_tangent);
	float dot_prod_tangent = dot (direction_to_light_tangent, normal_tangent);
	dot_prod_tangent = max (dot_prod_tangent, 0.0);
	

	//dot product for diffuse
	float dot_prod = dot(direction_to_light_eye, normal_eye);
	dot_prod = max(dot_prod, 0.0);	

	// Specular reflection	
	vec3 surface_to_viewer_eye = normalize(-position_eye);
	//Bling extension hafway eye space direction
	vec3 half_way_eye = normalize(surface_to_viewer_eye + direction_to_light_eye);
	float dot_specular = max(0.0,dot(half_way_eye, normal_eye));
	float spec_factor = pow(dot_specular, specular_exponent);

	//specular reflection tangents
	vec3 reflection_tangent = reflect (normalize (light_dir_tangent), normal_tangent);
	float dot_prod_specular = dot (reflection_tangent, normalize (view_dir_tangent));
	dot_prod_specular = max (dot_prod_specular, 0.0);
	float specular_factor_tangent = pow (dot_prod_specular, 100.0);


	// diffuse intensity
	vec3 Id = Ld * Kd * dot_prod;
	// specular intensity
	vec3 Is = Ls * Ks * spec_factor;	
	// ambient intensity
	vec3 Ia = La * Ka * 1.1;
	//Ia = diffuse.rgb * Ia;

	//fog
	const vec3 fog_color = vec3 (0.2, 0.2, 0.2);
	const float min_fog_radius = 3.0;
	const float max_fog_radius = 10.0;
	float dist = length(-position_eye);
	float fog_fac = (dist - min_fog_radius) / (max_fog_radius - min_fog_radius);
	fog_fac = clamp(fog_fac ,0.0, 1.0);
	//end fog

	//final color
	vec3 final_color = (Is + Id + Ia);
	final_color = mix(final_color, fog_color, fog_fac);
    fragment_color = vec4(final_color, 1.0);
}