#version 330 core
in vec2 TexCoords;

struct Light{
	vec3 lightPos;
	vec3 lightColor;

	float linear;
	float quadratic;
	float radius;
};

uniform Light lights[32];
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform vec3 cameraPos;

out vec4 FragColor;
void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).rgb;
	vec3 normal = texture(gPosition, TexCoords).rgb;
	vec3 diffuseColor = texture(gColor, TexCoords).rgb;
	float specularRation = texture(gColor, TexCoords).a;

	vec3 ambient = diffuseColor * 0.1;
	vec3 lighting = ambient;

	vec3 cameraDir = normalize(cameraPos - fragPos);
	for( int i = 0; i < 32; i++ )
	{
		float lightDistance = length(lights[i].lightPos - fragPos);
		if( lightDistance < lights[i].radius)
		{
			vec3 lightDir = normalize(lights[i].lightPos - fragPos);
			vec3 diffuse = max(dot(lightDir, normal), 0.0) * diffuseColor * lights[i].lightColor;

			vec3 halfway = normalize(lightDir + cameraDir);
			vec3 specular = pow(max(dot(halfway, normal), 0.0), 64) * specularRation * lights[i].lightColor;

			float attenuation = 1.0 / (1.0 + lights[i].linear * lightDistance + lights[i].quadratic * lightDistance * lightDistance);
			lighting += specular + diffuse;
			lighting *= attenuation;
		}
	}
	lighting = vec3(1.0) - exp(-lighting);
	lighting = pow(lighting, vec3(1.0 / 2.2));
	FragColor = vec4(lighting, 1.0);
}
