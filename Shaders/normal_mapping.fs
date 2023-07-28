#version 330 core

in VS_OUT{
	vec3 TangentLightPos;
	vec3 TangentCameraPos;
	vec3 TangentFragPos;
	vec2 TextCoords;
	mat3 TBN;
} fs_in;

uniform sampler2D diffuse;
uniform sampler2D normal;

out vec4 FragColor;

void main()
{
	vec3 Color = texture(diffuse, fs_in.TextCoords).rgb;

	vec3 Normal = texture(normal, fs_in.TextCoords).rgb;
	Normal = Normal * 2.0 - 1.0;

	vec3 LightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	vec3 CameraDir = normalize(fs_in.TangentCameraPos - fs_in.TangentFragPos);
	vec3 Half = normalize(LightDir + CameraDir);

	vec3 Ambient = 0.3 * Color;

	float Diff = max(dot(LightDir, Normal), 0);
	vec3 Diffuse = Diff * Color;

	float Spec = pow(max(dot(Half, Normal), 0), 2);
	vec3 Specular = Spec * Color;

	vec3 Light = (Ambient + Specular + Diffuse) * vec3(0.8);
	FragColor = vec4(Light, 1.0);
}