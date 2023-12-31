#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;

out vec2 TextCoords;
out vec3 FragPos;

void main()
{
	TextCoords = aTexture;
	FragPos = aPos;
	gl_Position = vec4(aPos, 1.0);
}
