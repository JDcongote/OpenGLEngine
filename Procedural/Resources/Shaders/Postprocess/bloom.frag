#version 460
layout (binding = 0) uniform sampler2D tex_sampler0;

out vec4 FragColor;

in vec2 tex_coords;

  
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 2.0 / textureSize(tex_sampler0, 0); // gets size of single texel
    vec3 result = texture(tex_sampler0, tex_coords).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 10; ++i)
        {
            result += texture(tex_sampler0, tex_coords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(tex_sampler0, tex_coords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 10; ++i)
        {
            result += texture(tex_sampler0, tex_coords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(tex_sampler0, tex_coords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}