#version 330 core
in VS_OUT{
	vec3 Normal;
	vec2 TextCoords;
	vec3 FragPos;
	vec4 FragPosLightSpace;
} vs_out;

uniform sampler2D diffuse_texture;
uniform sampler2D DepthMap;

uniform vec3 LightPos;
//uniform vec3 LightDir;
uniform vec3 CameraPos;

out vec4 FragColor;

float ShadowCal(vec4 FragPosLightSpace)
{
	vec3 ProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	ProjCoords = ProjCoords * 0.5 + 0.5;

	float ClosestDepth = texture(DepthMap, ProjCoords.xy).r;
	float CurrentDepth = ProjCoords.z;

	float shadow = CurrentDepth - 0.005 > ClosestDepth ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	vec3 Texture = texture(diffuse_texture, vs_out.TextCoords).xyz;
	vec3 Normal = normalize(vs_out.Normal);
	vec3 LightColor = vec3(1.0);
	// Ambient
	vec3 Ambient = 0.15 * LightColor;
	// Diffuse
	vec3 LightDir = normalize(LightPos - vs_out.FragPos);
	float diff = max(dot(LightDir, Normal), 0);
	vec3 Diffuse = diff * LightColor;
	// Specular
	vec3 CameraDir = normalize(CameraPos - vs_out.FragPos);
	vec3 HalfDir = normalize(CameraDir + LightDir);
	float spec = pow(max(dot(HalfDir, Normal), 0), 64.0);
	vec3 Specular = spec * LightColor;

	// º∆À„“ı”∞
	float shadow = ShadowCal(vs_out.FragPosLightSpace);
	vec3 Lighting = (Ambient + (1.0 - shadow) * (Diffuse + Specular)) * LightColor * Texture;

	FragColor = vec4(Lighting, 1.0);
	//FragColor = vec4(1.0);
}