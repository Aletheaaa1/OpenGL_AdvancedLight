#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 FragPos;
in vec3	Normal;
in vec2 TexCoords;

const float NEAR = 0.1f;
const float FAR = 50.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	z = (2.0 * NEAR * FAR) / ( FAR + NEAR - z * (FAR - NEAR));
	return z;
}

void main()
{
	gPosition.xyz = FragPos;
	gPosition.w = LinearizeDepth(gl_FragCoord.z);
	gNormal = Normal;
	gColor.rgb = vec3(0.95);
}
