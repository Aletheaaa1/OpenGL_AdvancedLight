#version 330 core
in vec2 TexCoords;

uniform sampler2D DepthMap;

out vec4 FragColor;
void main()
{
	float DepthValue = texture(DepthMap, TexCoords).x;
	FragColor = vec4(vec3(DepthValue), 1.0);
}