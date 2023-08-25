#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main()
{
	gPosition = FragPos;
	gNormal = Normal;
	gColor.xyz = texture(texture_diffuse, TexCoords).rgb;
	gColor.a = texture(texture_specular, TexCoords).r;
}
