#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

uniform vec3 LightPos;
uniform vec3 CameraPos;
uniform sampler2D texture_diffuse;
uniform samplerCube DepthMap;

out vec4 FragColor;

float ShadowCal(vec3 FragPos)
{
	vec3 LightDir = FragPos - LightPos;
	float Depth = texture(DepthMap, LightDir).r * 25.0;
	float CurrentDepth = length(LightDir);
	float bias = 0.05;
	FragColor = vec4(vec3(CurrentDepth / 25.0), 1.0);
	float shadow = CurrentDepth - bias > Depth? 1.0 : 0.0;
	return shadow;
}

void main()
{
	vec3 LightColor = vec3(1.0);
	vec4 Texture = texture(texture_diffuse, TextCoords);
	vec3 CameraDir = normalize(CameraPos - FragPos);
	vec3 LightDir = normalize(LightPos - FragPos);

	vec4 Ambient =vec4( 0.3 * LightColor, 1.0);

	float diff = max(dot(LightDir, Normal), 0);
	vec3 Diffuse = diff * LightColor;

	vec3 HalfVec = normalize(CameraDir + LightDir);
	float spec = pow(max(dot(HalfVec, Normal), 0), 64);
	vec3 Specular = spec * LightColor;

	float shadow = ShadowCal(FragPos);
		FragColor =(Ambient + (1 - shadow) * (vec4(Specular, 1.0) + vec4(Diffuse, 1.0))) * Texture;
}