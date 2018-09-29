#version 460
out vec4 FragColor;
  
in vec2 tex_coords;

layout (binding = 0) uniform sampler2D scene;
layout (binding = 1) uniform sampler2D bloomBlur;

uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, tex_coords).rgb;      
    vec3 bloomColor = texture(bloomBlur, tex_coords).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}  