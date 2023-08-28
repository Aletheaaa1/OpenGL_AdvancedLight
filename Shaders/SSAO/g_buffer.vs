#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in vec3 aTangnt;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out mat3 TBN;

void main()
{
	//	转到观察坐标系
	mat3 normalMatrix = transpose(inverse(mat3(view * model)));
	Normal = (normalMatrix * aNormal);
	vec3 t = (normalMatrix * aTangnt);
	vec3 b = cross(t, Normal);

	TBN = mat3(t, b, Normal);
	FragPos = (view * model * vec4(aPos, 1.0)).xyz;
	TexCoords = aTexture;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}