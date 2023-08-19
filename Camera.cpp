#include "Camera.h"

#include <iostream>
Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 world_up)
	: position(position), pitch(pitch), yaw(yaw), world_up(world_up)
{
	forword.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	forword.y = sin(glm::radians(pitch));
	forword.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	forword = -forword;
	target = position + forword;
	right = glm::normalize(glm::cross(forword, world_up));
	up = glm::normalize(glm::cross(right, forword));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, target, up);
}

void Camera::UpdateCameraView(float x_offset, float y_offset)
{
	pitch += y_offset * 0.03;
	yaw += x_offset * -0.03;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	forword.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	forword.y = sin(glm::radians(pitch));
	forword.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	forword = -forword;
	target = position + forword;
	right = glm::normalize(glm::cross(forword, world_up));
	up = glm::normalize(glm::cross(right, forword));
}

void Camera::UpdateCameraPosition()
{
	position += forword * -speedZ;
	position += glm::normalize(glm::cross(up, forword)) * -speedX;
	position += up * speedY;
	target = position + forword;
}