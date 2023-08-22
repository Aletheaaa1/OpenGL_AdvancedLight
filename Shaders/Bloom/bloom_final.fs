#version 330 core

in vec2 TextCoords;
in vec3 FragPos;

uniform sampler2D scene;
uniform sampler2D bloom;

out vec4 FragColor;
void main()
{
	vec3 color = texture(scene, TextCoords).xyz;
	vec3 bloom_color = texture(bloom, TextCoords).xyz;
	vec3 final_color = color + bloom_color;

	float gamma = 2.2;
	float exposure = 1.0;
	
	vec3 hdrColor = vec3(1.0) - exp(-final_color * exposure);
	hdrColor = pow(hdrColor, vec3(1.0 / gamma));

	FragColor = vec4(hdrColor, 1.0);
}