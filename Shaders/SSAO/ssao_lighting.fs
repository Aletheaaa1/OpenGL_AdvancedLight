#version 330 core
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D ssao;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform mat4 projection;

out vec4 FragColor;

float linear = 0.09;
float quadratic = 0.032;

void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = texture(gNormal, TexCoords).xyz;
	vec3 color = texture(gColor, TexCoords).rgb;
	float blur = texture(ssao, TexCoords).x;

	vec3 viewPos = vec3(projection * vec4(cameraPos, 1.0));
	vec3 lPos = vec3(projection * vec4(lightPos, 1.0));

	vec3 lightDir = normalize(lPos - fragPos);
	vec3 cameraDir = normalize(viewPos - fragPos);
	vec3 halfDir = normalize(lightDir + cameraDir);

	vec3 lighting = vec3(0.0);;
	vec3 ambient = 0.3 * blur * color;

	float diffuseRate = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = color * diffuseRate * lightColor;

	float specularRate = pow(max(dot(halfDir, normal) ,0.0), 32);
	vec3 specular = specularRate * color * lightColor;

	lighting += ambient + diffuse + specular;
	lighting = pow(lighting, vec3(1.0 / 2.2));

	float dist = length(lPos - fragPos);
	float attenuation = 1.0 / (dist * dist * quadratic);
	lighting *= attenuation;

	FragColor = vec4(vec3(blur), 1.0);
}