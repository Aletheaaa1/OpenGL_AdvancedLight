#version 330 core
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform mat4 projection;
uniform vec3 kernels[64];

out vec4 FragColor;

void main()
{
	vec3 normal = texture(gNormal, TexCoords).xyz;
	vec3 fragPos = texture(gPosition, TexCoords).rgb;	// fragPos为观察空间
	vec3 color = texture(gColor, TexCoords).rgb;

	vec3 randomVec = vec3(1.0);
	vec3 t = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 b = cross(normal, t);
	mat3 TBN = mat3(t, b, normal);	// normal为观察空间

	float aoWeight = 0.0;
	for(int i = 0; i < 64; i++)
	{
		vec3 kernel = TBN * kernels[i];
		kernel = kernel + fragPos;

		//	转换到裁切空间
		vec4 kernelPos = projection * vec4(kernel, 1.0);
		//	NDC化
		kernelPos.xyz /= kernelPos.w;
		kernelPos.xyz = kernelPos.xyz * 0.5 + 0.5;

		float depth = texture(gPosition, kernelPos.xy).z;

		float rangeCheck = smoothstep(0.0, 1.0, 0.2 / abs(fragPos.z - depth));

		float perWeight = (depth > kernel.z ? 1.0 : 0.0) * rangeCheck;
		aoWeight += perWeight;
	}
	aoWeight = aoWeight / 64.0;
	FragColor = vec4(vec3(1.0 - aoWeight), 1.0);
}