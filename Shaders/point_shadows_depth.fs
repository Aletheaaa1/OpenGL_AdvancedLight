#version 330 core

in vec4 FragPos;

uniform vec3 LightPos;

void main()
{
	float dis = length(LightPos - FragPos.xyz);
	gl_FragDepth = dis / 25.0;
}