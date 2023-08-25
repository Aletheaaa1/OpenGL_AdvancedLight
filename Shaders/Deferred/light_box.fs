#version 330 core
layout (location = 0) out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
	vec3 light;
	light =  vec3(1.0) - exp(-lightColor * 1);
	light = pow(light, vec3(1.0 / 2.2));
	FragColor = vec4(light, 1.0);
}
