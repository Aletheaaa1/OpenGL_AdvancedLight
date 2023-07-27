#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexture;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 LightPos;
uniform vec3 CameraPos;

out vec3 normal;
out vec3 TangentLightPos;
out vec3 TangentCameraPos;
out vec3 TangentFragPos;
out vec2 TangentTextCoords;
out mat3 TBN;

void main()
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));

	vec3 T = normalize(transpose(inverse(mat3(model))) * aTangent);
	vec3 B = normalize(transpose(inverse(mat3(model))) * aBitangent);
	vec3 N = normalize(transpose(inverse(mat3(model))) * aNormal);
	TBN = transpose(mat3(T, B, N));

	TangentLightPos = TBN * LightPos;
	TangentCameraPos = TBN * CameraPos;
	TangentFragPos = TBN * FragPos;
	TangentTextCoords = aTexture;

	normal =  normalize(transpose(inverse(mat3(model))) * aNormal);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
