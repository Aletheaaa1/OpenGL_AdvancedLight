#version 330 core
in vec2 TexCoords;

struct Light{
	vec3 lightPos;
	vec3 lightColor;

	float linear;
	float quadratic;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform Light lights[32];
uniform vec3 CameraPos;

out vec4 FragColor;

void main()
{
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gColor, TexCoords).rgb;
	float Specular = texture(gColor, TexCoords).a;

	vec3 cameraDir = normalize(CameraPos - FragPos); 
	vec3 ambient = 0.1 * Diffuse;
	vec3 lighting = ambient;
	for(int i=0; i<32; i++)
	{
		vec3 lightDir = normalize(lights[i].lightPos - FragPos);
		vec3 diffuse = max(dot(lightDir, Normal), 0.0) * Diffuse * lights[i].lightColor;

		vec3 halfWay = normalize(cameraDir + lightDir);
		vec3 specular = pow((max(dot(halfWay, Normal), 0.0)), 128) * Specular * lights[i].lightColor;

		lighting += diffuse + specular ;
		lighting *= (1.0 / lights[i].quadratic * lights[i].quadratic);
	}
	
	lighting =  vec3(1.0) - exp(-lighting * 1.0);
	lighting = pow(lighting, vec3(1.0 / 2.2));
	
	FragColor = vec4(lighting, 1.0);
}