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

out VS_OUT{
	vec3 TangentLightPos;
	vec3 TangentCameraPos;
	vec3 TangentFragPos;
	vec2 TextCoords;
	mat3 TBN;
} vs_out;

void main()
{
	mat3 NormalMat = transpose(inverse(mat3(model)));

	vec3 T = normalize(NormalMat * aTangent);
	vec3 B = normalize(NormalMat * aBitangent);
	vec3 N = normalize(NormalMat * aNormal);
	vs_out.TBN = transpose(mat3(T, B, N));

	vs_out.TangentLightPos = vs_out.TBN * LightPos;
	vs_out.TangentCameraPos = vs_out.TBN * CameraPos;
	vs_out.TangentFragPos = vs_out.TBN *vec3(model * vec4(aPos, 1.0));

	vs_out.TextCoords = aTexture;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
