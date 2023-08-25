#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	mat3 normalMat = transpose(inverse(mat3(model)));
	Normal = normalize(normalMat * aNormal);

	TexCoords = aTexture;

	FragPos =vec3(model * vec4(aPos, 1.0));

	gl_Position = projection * view * model * vec4(aPos, 1);
}
