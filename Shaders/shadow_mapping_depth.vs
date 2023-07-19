#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 LightSpaceMatrix;
uniform mat4 model;

out vec4 FragCoords;

void main()
{
	FragCoords = model * vec4(aPos, 1.0);
	gl_Position = LightSpaceMatrix * model * vec4(aPos, 1.0);
}
