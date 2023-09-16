#version 330 core
in vec3 FragPos;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
	vec3 randomVec = vec3(0.0, 1.0, 0.0);
	vec3 N = normalize(FragPos);
//	vec3 T = normalize(N - N * dot(N, randomVec));
	vec3 T = normalize(cross(N,randomVec));
	vec3 B = normalize(cross(T, N));

	vec3 irradiance = vec3(0.0);

	const float PI = 3.1415926;
	float samplerDelta = 0.025;
	float nrSampler = 0.0;
	for(float phi = 0.0; phi < 2.0 * PI; phi += samplerDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += samplerDelta)
		{
			float x = sin(theta) * cos(phi);
			float z = sin(theta) * sin(phi);
			float y = cos(theta);

			//	与TBN对应，按照demo案例时需要改变TBN坐标轴方向
			vec3 samplerVec = x * T + y * N + z * B;
			irradiance += texture(skybox, samplerVec).rgb  * cos(theta);
			nrSampler++;
		}
	}

	irradiance = PI * irradiance * (1.0 / float(nrSampler));
	FragColor = vec4(irradiance, 1.0);
}