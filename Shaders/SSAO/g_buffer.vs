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
	//	提取出来的线性深度是在观察空间中的，所以之后的运算也是在观察空间中
	mat3 normalMatrix = transpose(inverse(mat3(view * model)));
	Normal = normalize(normalMatrix * aNormal);
	FragPos = vec3(view * model * vec4(aPos, 1.0));
	TexCoords = aTexture;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}