#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	FragPos = aPos;

	vec4 pos = projection  * view * vec4(aPos, 1.0);
	gl_Position = pos.xyzw;
}