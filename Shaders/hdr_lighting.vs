#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TextCoords;
} vs_out;

void main()
{
	vs_out.TextCoords = aTexture;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

	mat3 transposeMat = transpose(inverse(mat3(model)));
	vs_out.Normal = normalize(transposeMat * -aNormal);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
