#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightPos;
uniform vec3 CameraPos;

out VS_OUT{
	mat3 TBN;
	vec3 TanLightPos;
	vec3 TanCameraPos;
	vec3 TanFragPos;
	vec2 TextCoords;
} vs_out;

void main()
{
	mat3 NormalMat = transpose(inverse(mat3(model)));

	vec3 T = normalize(NormalMat * aTangent);
	vec3 B = normalize(NormalMat * aBitangent);
	vec3 N = normalize(NormalMat * aNormal);
	vs_out.TBN = transpose(mat3(T, B, N));

	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TanFragPos = vs_out.TBN * FragPos;
	vs_out.TanLightPos = vs_out.TBN * LightPos;
	vs_out.TanCameraPos = vs_out.TBN * CameraPos;
	vs_out.TextCoords = aTexture;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}