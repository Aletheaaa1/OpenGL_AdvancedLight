#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera
{
  public:
    glm::vec3 forword;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 world_up;
    glm::vec3 right;

    float pitch;
    float yaw;

    float speedZ;
    float speedX;
    float speedY;

  public:
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 world_up);

    glm::mat4 GetViewMatrix();

    void UpdateCameraView(float x_offset, float y_offset);

    void UpdateCameraPosition();
};