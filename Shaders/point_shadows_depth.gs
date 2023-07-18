#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 ShadowMat[6];

out vec4 FragPos;

void main()
{
	for(int face=0; face<6; face++)
	{
		gl_Layer = face;
		for(int j=0; j<3;j++)
		{
			FragPos = gl_in[j].gl_Position;
			gl_Position = ShadowMat[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}