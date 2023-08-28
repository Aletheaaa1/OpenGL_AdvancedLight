#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

void main()
{
	vec3 normalVS = texture(texture_normal, TexCoords).rgb;
 	normalVS = normalVS * 2.0 - 1.0;
	normalVS = normalize(TBN * normalVS);

	gPosition = FragPos;
	gNormal = normalVS;
	gColor = texture(texture_diffuse, TexCoords);
}