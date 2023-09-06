#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in vec3 aT;
layout (location = 4) in vec3 aB;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 T;
out vec3 B;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	TexCoords = aTexture;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	Normal = normalize(normalMatrix * aNormal);
	FragPos = vec3(model * vec4(aPos, 1.0));

	T = aT;
	B = aT;
}