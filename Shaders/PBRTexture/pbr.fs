#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 T;
in vec3 B;

struct Material
{
	float roughness;
	float metallic;
	vec3 albedo;
};
struct Light
{
	vec3 lightPos;
	vec3 lightColor;
};

uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D roughness_texture;
uniform sampler2D metallic_texture;
uniform sampler2D ao_texture;

uniform Light lights[4];
uniform Material material;
uniform vec3 cameraPos;

out vec4 FragColor;

const float PI = 3.14159265359;

//	NDF
float NDF(vec3 n, vec3 h, float roughness)
{
	//	in GGX, α equals to pow roughness
	float a = roughness * roughness;

	float numerator = a * a;
	float denominator = pow(max(dot(n, h), 0), 2) * (a * a - 1) + 1;
	denominator = PI * denominator * denominator;
	return numerator / denominator;
}

//	Geometry
float GeometrySchlick(float result, float roughness)
{
	float k = pow((roughness + 1), 2) / 8;
	float numerator = result;
	float denominator = result * (1 - k) + k;

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

//	FernelSchlick
vec3 FernelSchlick(vec3 h, vec3 v, float metallic, vec3 albedo)
{
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metallic);
	float HdotV = max(dot(h, v), 0.0);
	HdotV = clamp(HdotV, 0.0, 1.0);

	return f0 + (1 - f0) * pow((1 - HdotV), 5);
}

void main()
{
	//	TBN
	vec3 n = normalize(Normal);
	vec3 t = normalize(T);
	vec3 b = normalize(B);
	mat3 TBN = mat3(t, b, n);

	vec3 albedoColor = texture(albedo_texture, TexCoords).xyz;
	vec3 normalColor = texture(normal_texture, TexCoords).xyz * 2.0 - 1.0;
	normalColor = normalize(TBN * normalColor);
	vec3 roughnessColor = texture(roughness_texture, TexCoords).xyz;
	vec3 metallicColor = texture(metallic_texture, TexCoords).xyz;
	vec3 aoColor = texture(ao_texture, TexCoords).xyz;

	vec3 result = vec3(0.0);

	for(int i=0; i<4; i++)
	{
		vec3 lightDir = normalize(lights[i].lightPos - FragPos);
		vec3 cameraDir = normalize(cameraPos - FragPos);
		vec3 halfDir = normalize(lightDir + cameraDir);
		float hDotV = max(dot(halfDir, cameraDir), 0);

		float D = NDF(normalColor, halfDir, roughnessColor.x);
		float G = GeometrySmith(normalColor, cameraDir, lightDir, roughnessColor.x);
		vec3 F = FernelSchlick(halfDir, cameraDir, metallicColor.x, albedoColor);

		//	BRDF
		vec3 brdfNumerator = D * F * G;
		float brdfDenominator = 4.0 * max(dot(normalColor, cameraDir), 0.0) * max(dot(normalColor, lightDir), 0.0) + 0.00001;
		vec3 specular = brdfNumerator / brdfDenominator;

		vec3 kS = specular;
		vec3 kD = 1 - kS;
		kD = kD * (1 - metallicColor.x);

		//	反色率方程
		vec3 Lo = (kD * albedoColor / PI + kS) * lights[i].lightColor * max(dot(normalColor, lightDir), 0.0);
		result += Lo;
	}

	vec3 ambient = 0.01 * albedoColor * aoColor.x;

	result += ambient;
	result = vec3(1.0) - exp(-result * 1.0);
	result = pow(result, vec3(1.0 / 2.2));
	FragColor = vec4(result, 1.0);
}