#version 330 core
in vec3 TangentLightPos;
in vec3 TangentCameraPos;
in vec2 TangentTextCoords;
in vec3 TangentFragPos;
in vec3 normal;
in mat3 TBN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

out vec4 FragColor;


void main()
{
	vec3 Color = texture(texture_diffuse, TangentTextCoords).rgb;
	vec3 Normal = texture(texture_normal, TangentTextCoords).rgb;
	Normal = Normal * 2.0 - 1.0;

	vec3 LightDir = normalize(TangentLightPos - TangentFragPos);

	vec3 Ambient = Color * 0.1;

	float Diff = max(dot(LightDir, Normal), 0.0);
	vec3 Diffuse = Diff * Color;

	vec3 CameraDir = normalize(TangentCameraPos - TangentFragPos);
	vec3 Half = normalize(LightDir + CameraDir);
	float Spec = pow(max(dot(Normal, Half), 0), 32.0);
	vec3 Specular = Spec * Color * 0.3;

	vec3 Light = (Ambient + Diffuse + Specular) * vec3(1.0);
	FragColor = vec4(Light, 1.0);
}