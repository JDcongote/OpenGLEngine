//fragment.frag
#version 400

//in vec3 color;
in vec3 light_pos, color, position_eye, normal_eye, normals;
in mat4 view_mat;

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
float specular_exponent = 10.0; // specular 'power'

void main() {
	// ambient intensity
	vec3 Ia = La * Ka;

	//light position in eye space
	vec3 light_eye = vec3(view_mat * vec4(light_pos, 1.0));
	vec3 distance_to_light = light_eye - position_eye;
	vec3 direction_to_light = normalize(distance_to_light);
	float dot_pr = dot(direction_to_light, normal_eye);
	dot_pr = max(dot_pr, 0.0);

	// diffuse intensity
	vec3 Id = Ld * Kd * dot_pr;

	// Specular intensity
	vec3 surface_to_eye = normalize(-position_eye);
	//Bling extension
	vec3 half_way_eye = normalize(surface_to_eye + direction_to_light);

	float dot_specular = max(0.0,dot(half_way_eye, normal_eye));
	float spec_factor = pow(dot_specular, specular_exponent);

	vec3 Is = Ls * Ks * spec_factor;


    fragment_color = vec4(Is + Id + Ia, 1.0);
}