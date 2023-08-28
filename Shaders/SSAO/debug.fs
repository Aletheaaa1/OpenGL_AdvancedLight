#version 330 core
in vec2 TexCoords;

uniform sampler2D texture_test;

out vec4 FragColor;

void main()
{
	vec3 color = texture(texture_test, TexCoords).xyz;
//	color = vec3(1.0) - exp(-color * 1.0);
//	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}