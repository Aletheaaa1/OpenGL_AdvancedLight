#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

class LightDirectional
{
public:
	glm::vec3 m_position;
	glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_color;
	glm::vec3 m_angles;

public:
	LightDirectional(glm::vec3 position, glm::vec3 angles, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

	void UpdateDirection();

	std::initializer_list<float> GetPosition();
	std::initializer_list<float> GetDirection();
	std::initializer_list<float> GetColor();
};
