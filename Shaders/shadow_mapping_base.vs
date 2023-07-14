#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

out VS_OUT{
	vec3 Normal;
	vec2 TextCoords;
	vec3 FragPos;
	vec4 FragPosLightSpace;
} vs_out;

void main()
{
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TextCoords = aTexture;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}