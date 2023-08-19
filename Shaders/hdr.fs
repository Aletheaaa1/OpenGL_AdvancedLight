#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{             
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    // vec3 result = hdrColor / (1.0 + hdrColor);
    vec3 result = vec3(1.0) - exp(-hdrColor * 1.0);
    result = pow(result, vec3(1.0 / 2.2));
    FragColor = vec4(result, 1.0);
}