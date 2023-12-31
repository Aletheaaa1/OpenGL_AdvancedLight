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

float ShadowCal(vec4 FragPosLightSpace, vec3 LightDir, vec3 Normal)
{
	vec3 ProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	ProjCoords = ProjCoords * 0.5 + 0.5;

	float CurrentDepth = ProjCoords.z;
	float bias = max(0.01 * dot(Normal, LightDir), 0.005);

	float shadow = 0.0;
	vec2 TextureSize = 1.0 / textureSize(DepthMap, 0);
	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			float PCFDepth = texture(DepthMap, ProjCoords.xy + vec2(x, y) * TextureSize).x;
			shadow += CurrentDepth - bias > PCFDepth ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;
	
	if(ProjCoords.z > 1.0)
	{
		shadow = 0;
	}

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

	// ������Ӱ
	float shadow = ShadowCal(vs_out.FragPosLightSpace, LightDir, Normal);
	vec3 Lighting = (Ambient + (1.0 - shadow) * (Diffuse + Specular)) * LightColor * Texture;

	FragColor = vec4(Lighting, 1.0);
	//FragColor = vec4(1.0);
}