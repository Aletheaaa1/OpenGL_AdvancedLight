#version 330 core

struct Light {
    vec3 LightPos;
	vec3 LightColor;
};

uniform Light lights[4];
uniform sampler2D texture_diffuse;
uniform vec3 CameraPos;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TextCoords;
} fs_in;

out vec4 FragColor;
void main()
{
	vec3 color = texture(texture_diffuse, fs_in.TextCoords).rgb;
	vec3 normal = fs_in.Normal;
	vec3 ambient = vec3(0.0);
	vec3 lighting = vec3(0.0);

	for(int i=0; i<4; i++)
	{
		vec3 LightDir = normalize(lights[i].LightPos - fs_in.FragPos);
		float diff = max(dot(LightDir, normal), 0.0);
		vec3 diffuse = lights[i].LightColor * diff * color;

		float distance = length(fs_in.FragPos - lights[i].LightPos);
        diffuse *= 1.0 / (distance * distance);
		lighting += diffuse;
	}
	FragColor = vec4(lighting + ambient, 1);
}