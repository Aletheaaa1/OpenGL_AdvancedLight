#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material
{
	vec3 albedo;
	float roughness;
	float metallic;
};

struct Light
{
	vec3 lightPos;
	vec3 lightColor;
};

uniform Light lights[4];
uniform Material material;
uniform vec3 cameraPos;

//	PBR
//	------------------------------------------------
const float PI = 3.14159265359;
//	NDF
float NDF(vec3 n, vec3 h, float roughness)
{
	//	in GGX, ¦Á equals to pow roughness
	float a = roughness * roughness;

	float numerator = a * a;
	float denominator = pow(max(dot(n, h), 0), 2) * (a * a - 1) + 1;
	denominator = PI * denominator * denominator;
	return numerator / denominator;
}

//	Geometry
float GeometrySchlick(float dotResult, float roughness)
{
	float k =  pow((roughness + 1.0), 2) / 8.0;
	float numerator = dotResult;
	float denominator = dotResult * (1.0 - k) + k;

	return numerator / denominator;
}

float GeometrySmith(vec3 n, vec3 v, vec3 l, float roughness)
{
	float NdotV = max(dot(n, v), 0.0);
	float NdotL = max(dot(n, l), 0.0);

	float ggx1 = GeometrySchlick(NdotV, roughness);
	float ggx2 = GeometrySchlick(NdotL, roughness);

	return ggx1 * ggx2;
}

//	Fernel
vec3 Fernel(vec3 h, vec3 v, float metallic)
{
	vec3 f0 = vec3(0.56, 0.57, 0.58);
	f0 = mix(f0, material.albedo, metallic);
	float HdotV = max(dot(h, v), 0.0);
	HdotV = clamp(HdotV, 0.0, 1.0);

	return f0 + (1 - f0) * pow(clamp((1 - HdotV), 0.0, 0.5), 5);
}

void main()
{
	vec3 result = vec3(0.0f);
	vec3 normal = normalize(Normal);

	for(int i = 0; i < 4; i++)
	{
		vec3 lightDir = normalize(lights[i].lightPos - FragPos);
		vec3 viewDir = normalize(cameraPos - FragPos);
		vec3 halfDir = normalize(lightDir + viewDir);

		float D = NDF(normal, halfDir, material.roughness);
		float G = GeometrySmith(normal, viewDir, lightDir, material.roughness);
		vec3 F = Fernel(halfDir, viewDir, material.metallic);

		vec3 BRDFNumerator = D * G * F;
		float BRDFDenominator = 4.0 * max(dot(normal, viewDir), 0) * max(dot(normal, lightDir), 0) + 0.0001;
		vec3 BRDF = BRDFNumerator / BRDFDenominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= (1.0 - material.metallic);

		vec3 Lo = (kD * material.albedo / PI + BRDF) * lights[i].lightColor * max(dot(normal, lightDir), 0.0);

		result += Lo;
	}

	vec3 ambient = 0.05 * material.albedo;

	result += ambient;
	result = vec3(1.0f) - exp(-result * 1.0);
	result = pow(result, vec3(1.0 / 2.2));

	FragColor = vec4(result ,1.0);
}