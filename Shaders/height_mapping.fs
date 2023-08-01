#version 330 core

in VS_OUT{
	mat3 TBN;
	vec3 TanLightPos;
	vec3 TanCameraPos;
	vec3 TanFragPos;
	vec2 TextCoords;
} fs_in;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D height;
uniform float height_scale;

out vec4 FragColor;

vec2 HeightMapping(vec2 TextCoords, vec3 CameraDir)
{
	float h = texture(height, TextCoords).r;
	vec2 offset = CameraDir.xy / CameraDir.z * (h * height_scale);
	return TextCoords - offset;
}

void main()
{
	vec3 CameraDir = normalize(fs_in.TanCameraPos - fs_in.TanFragPos);
	vec3 LightDir = normalize(fs_in.TanLightPos - fs_in.TanFragPos);

	vec2 TextCoords = HeightMapping(fs_in.TextCoords, CameraDir);
	if(TextCoords.x > 1.0 ||TextCoords.x < 0.0 || TextCoords.y > 1.0 || TextCoords.y < 0.0)
	{
		discard;
	}

	vec3 Color = texture(diffuse, TextCoords).xyz;

	vec3 Normal = texture(normal, TextCoords).rgb;
	Normal = Normal * 2.0 - 1.0;

	vec3 Ambient = 0.05 * Color;

	float Diff = max(dot(LightDir, Normal), 0.0);
	vec3 Diffuse = Diff * Color;

	vec3 Half = normalize(LightDir + CameraDir);
	float Spec = pow(max(dot(Normal, Half), 0.0), 32.0);
	vec3 Specular = Spec * Color;

	vec3 Light = (Ambient + Diffuse + Specular) * vec3(0.6);
	FragColor = vec4(Light, 1.0);
}