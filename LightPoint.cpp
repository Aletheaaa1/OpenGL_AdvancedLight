#include "LightPoint.h"

LightPoint::LightPoint(glm::vec3 position, glm::vec3 color)
	:m_position(position), m_color(color)
{
}

std::initializer_list<float> LightPoint::GetPosition()
{
	return { m_position.x, m_position.y, m_position.z };
}

std::initializer_list<float> LightPoint::GetColor()
{
	return { m_color.x, m_color.y, m_color.z };
}