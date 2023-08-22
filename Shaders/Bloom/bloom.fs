#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Light{
	vec3 LightPos;
	vec3 LightColor;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoords;

uniform vec3 CameraPos;
uniform Light lights[4];
uniform sampler2D texture_diffuse;

void main()
{
	vec3 color = texture(texture_diffuse, TextCoords).rgb;
	
	vec3 ambient = vec3(0.0);
	vec3 lighting = vec3(0.0);
	vec3 cameraDir = normalize(CameraPos - FragPos);

	for(int i=0; i<4; i++)
	{
		vec3 lightDir = normalize(lights[i].LightPos - FragPos); 
		float diff = max(dot(lightDir, Normal), 0.0);
		vec3 diffuse = diff * lights[i].LightColor * color;
		float distance = length(FragPos - lights[i].LightPos);
		diffuse *= 1.0 / (distance * distance);
		lighting += diffuse;
	}

	float brightness = dot(lighting, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
	{
		BrightColor = vec4(lighting, 1.0);
	}
	else
	{
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}

	FragColor = vec4(lighting + ambient, 1.0);
}