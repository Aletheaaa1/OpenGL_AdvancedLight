#pragma once
#include <iostream>
#include <glm/glm.hpp>
class LightPoint
{
public:
	glm::vec3 m_position;
	glm::vec3 m_color;
	const float constant{ 1.0f };
	const float linear{ 0.09f };
	const float quadratic{ 0.032f };
public:
	LightPoint(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

	std::initializer_list<float> GetPosition();
	std::initializer_list<float> GetColor();
};
