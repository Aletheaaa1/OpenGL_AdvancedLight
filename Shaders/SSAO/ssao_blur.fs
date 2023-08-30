#version 330 core
in vec2 TexCoords;

uniform sampler2D texture_ssao;

out float FragColor;
void main()
{
	vec3 ssao = texture(texture_ssao, TexCoords).xyz;
	vec2 texelSize = 1.0 / vec2(textureSize(texture_ssao, 0));
	float result = 0.0;
	for(int x = -2; x < 2; x++)
	{
		for(int y = -2; y < 2; y++)
		{
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(texture_ssao, TexCoords + offset ).r;
		}
	}
	FragColor = result / 16;
	//FragColor = vec4(ssao, 1.0);
}