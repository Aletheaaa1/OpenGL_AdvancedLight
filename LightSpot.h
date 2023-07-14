#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

class LightSpot
{
public:
	glm::vec3 m_position;
	glm::vec3 m_angles;
	glm::vec3 m_direction;
	glm::vec3 m_color;
	float m_cosPhi;
	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };

public:
	LightSpot(glm::vec3 position, glm::vec3 angles, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float cosPhi = 0.95f);

	void UpdateDirection();

	std::initializer_list<float> GetPosition();
	std::initializer_list<float> GetColor();
	std::initializer_list<float> GetDirection();
};