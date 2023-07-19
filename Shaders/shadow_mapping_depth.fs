#version 330 core

in vec4 FragPos;

uniform vec3 LightPos;

void main()
{
	gl_FragDepth = gl_FragCoord.z / gl_FragCoord.w;
}