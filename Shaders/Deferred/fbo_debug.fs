#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

void main()
{
	FragColor = texture(texture_normal, TexCoords);
}
