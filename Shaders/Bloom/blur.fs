#version 330 core
out vec4 FragColor;

in vec2 TextCoords;

uniform sampler2D Scene;
//	Gaussian Blur Weight
uniform float Weight[5]	= float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
uniform bool Horizontal;

void main()
{
	vec2 texture_offset = 1.0 / textureSize(Scene, 0);
	vec3 result = texture(Scene, TextCoords).rgb * Weight[0];
	if(Horizontal == true)
	{
		for(int i = 0; i < 5; i++)
		{
			result += texture(Scene, TextCoords + vec2(texture_offset.x * i, 0.0)).rgb * Weight[i];
			result += texture(Scene, TextCoords - vec2(texture_offset.x * i, 0.0)).rgb * Weight[i];
		}
	}
	else
	{
		for(int i = 0; i < 5; i++)
		{
			result += texture(Scene, TextCoords + vec2(0.0, texture_offset.y * i)).rgb * Weight[i];
			result += texture(Scene, TextCoords - vec2(0.0, texture_offset.y * i)).rgb * Weight[i];
		}
	}

	FragColor = vec4(result, 0);
}


