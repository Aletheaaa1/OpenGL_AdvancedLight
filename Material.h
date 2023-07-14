#pragma once

class Material
{
public:
	float* m_ambient;
	float* m_diffuse;
	unsigned int m_material_diffuse;
	float* m_specular;
	unsigned int m_material_specular;
	float m_shininess;

public:
	Material(float* ambient, float* diffuse, float* specular, float shininess);
	Material(float* ambient, unsigned int diffuse, unsigned int specular, float shininess);

	float* GetAmbient();
	float* GetDiffuse();
	float* GetSpecular();
	float GetShininess();
	unsigned int GetMaterialDiffuse();
	unsigned int GetMaterialSpecular();
};