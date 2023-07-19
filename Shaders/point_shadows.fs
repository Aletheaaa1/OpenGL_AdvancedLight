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
	float shadow = 0.0;

	vec3 LightDir = FragPos - LightPos;
	float CurrentDepth = length(LightDir) / 25.0;

	// 传统PCF
	/*
	float samples = 4;
	float offset = 0.1;
	for(float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for(float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for(float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float Depth = texture(DepthMap, LightDir + vec3(x, y, z)).r;
				if(CurrentDepth - 0.05 > Depth)
				{
					shadow += 1.0;
				}
			}
		}
	}
	*/

	// 改进PCF
	const int num = 20;
	vec3 sampleOffsetDirections[20] = vec3[]
	(
		vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
		vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
		vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
		vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
		vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);
	for(int i=0;i<num;i++)
	{
		float Depth = texture(DepthMap, LightDir + sampleOffsetDirections[i] * 0.05).r;
			if(CurrentDepth - 0.05 > Depth)
				{
					shadow += 1.0;
				}
	}

	shadow /= num;		

	return shadow;
}

void main()
{
	float shadow = ShadowCal(FragPos);

	vec3 LightColor = vec3(1.0) * 0.3;
	vec3 Color = texture(texture_diffuse, TextCoords).xyz;

	// ambient 
	vec3 Ambient = 0.3 * LightColor;

	// diffuse
	vec3 LightDir = normalize(LightPos - FragPos);
	float diff = max(dot(LightDir, Normal), 0.0);
	vec3 Diffuse = diff * LightColor;

	// specular
	vec3 CameraDir = normalize(CameraPos - FragPos);
	vec3 HalfDir = normalize(LightDir + CameraDir);
	float spec = pow(max(dot(Normal, HalfDir), 0.0), 64.0);
    vec3 Specular = spec * LightColor;    

	vec3 Light =(Ambient + (1 - shadow ) * (Diffuse + Specular))* Color;
	FragColor = vec4(Light, 0);
}