#include "LightDirectional.h"

LightDirectional::LightDirectional(glm::vec3 position, glm::vec3 angles, glm::vec3 color)
	:m_position(position), m_angles(angles), m_color(color)
{
	UpdateDirection();
}

void LightDirectional::UpdateDirection()
{
	m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	m_direction = glm::rotateX(m_direction, m_angles.x);
	m_direction = glm::rotateY(m_direction, m_angles.y);
	m_direction = glm::rotateZ(m_direction, m_angles.z);
	m_direction = glm::normalize(m_direction);
	m_direction *= -1;
}

std::initializer_list<float> LightDirectional::GetPosition()
{
	return { m_position.x, m_position.y, m_position.z };
}

std::initializer_list<float> LightDirectional::GetDirection()
{
	return { m_direction.x, m_direction.y, m_direction.z };
}

std::initializer_list<float> LightDirectional::GetColor()
{
	return { m_color.x, m_color.y, m_color.z };
}