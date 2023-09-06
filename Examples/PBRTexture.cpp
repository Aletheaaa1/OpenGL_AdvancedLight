//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include "../VertexArrayObject.h"
#include "../VertexBufferObject.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../Model.h"
#include "../Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <format>
#include <random>
#include <memory>

Camera camera{ glm::vec3(0.0f, 0.0f, 5.0f), 0.0f, 0.0f, glm::vec3(0, 1.0f, 0) };
constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
constexpr unsigned int SCREEN_WIDHT = 800, SCREEN_HEIGHT = 600;

#pragma region Data
float quadVertices[] = {
	// positions						  // texture Coords
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
};

float vertices[] = {
	// back face
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
	// front face
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	// left face
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	// right face
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
	 // bottom face
	 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
	  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	 // top face
	 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
	  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
};
#pragma endregion Data

#pragma region Interaction
float x_last, y_last;
bool first_mouse = true;
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (first_mouse == true)
	{
		x_last = x_pos;
		y_last = y_pos;
		first_mouse = false;
	}

	float x_offset = x_pos - x_last;
	float y_offset = y_pos - y_last;

	x_last = x_pos;
	y_last = y_pos;

	camera.UpdateCameraView(x_offset, y_offset);
	camera.GetViewMatrix();
}

void ProcessInput(GLFWwindow* window)
{
	float velocity = 0.005f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.speedZ = -velocity;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.speedZ = velocity;
	}
	else
	{
		camera.speedZ = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.speedX = -velocity;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.speedX = velocity;
	}
	else
	{
		camera.speedX = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.speedY = velocity;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.speedY = -velocity;
	}
	else
	{
		camera.speedY = 0.0f;
	}
}

bool reload_shader;
void ReLoad(GLFWwindow* window, int focus)
{
	if (focus == 1)
	{
		reload_shader = true;
	}
	else
	{
		reload_shader = false;
	}
}
#pragma endregion Interaction

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "glad error!" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

#pragma region Shaders
	Shader shader{ "./Shaders/PBRTexture/pbr.vs", "./Shaders/PBRTexture/pbr.fs" };
#pragma endregion Shaders

#pragma region Models
	Model ball{ "./Models/Planet/planet.obj" ,false };
#pragma endregion Models

#pragma region Texture
	Texture albedo{ "./Textures/pbr/rusted_iron/albedo.png", true };
	Texture roughness{ "./Textures/pbr/rusted_iron/roughness.png", false };
	Texture metallic{ "./Textures/pbr/rusted_iron/metallic.png", false };
	Texture normal{ "./Textures/pbr/rusted_iron/normal.png", false };
	Texture ao{ "./Textures/pbr/rusted_iron/ao.png", false };
#pragma endregion

#pragma region Light
	glm::vec3 lightPositions[] =
	{
			glm::vec3(-10.0f,  10.0f, 10.0f),
			glm::vec3(10.0f,  10.0f, 10.0f),
			glm::vec3(-10.0f, -10.0f, 10.0f),
			glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] =
	{
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(3.0f, 3.0f, 3.0f)
	};
#pragma endregion Light

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		auto start = glfwGetTime();
		ProcessInput(window);
		glfwSetWindowFocusCallback(window, ReLoad);
		if (reload_shader == true)
		{
			Shader new_shader{ "./Shaders/PBRTexture/pbr.vs", "./Shaders/PBRTexture/pbr.fs" };
			shader = new_shader;
			reload_shader = false;
		}

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(0.1f));
		albedo.Usetexture(0);
		albedo.Bind();
		normal.Usetexture(1);
		normal.Bind();
		roughness.Usetexture(2);
		roughness.Bind();
		metallic.Usetexture(3);
		metallic.Bind();
		ao.Usetexture(4);
		ao.Bind();

		shader.Bind();
		shader.SetUniformMat4("projection", projection);
		shader.SetUniformMat4("view", view);
		for (unsigned int i = 0; i < 4; i++)
		{
			shader.SetUniform3v("lights[" + std::to_string(i) + "].lightPos", lightPositions[i]);
			shader.SetUniform3v("lights[" + std::to_string(i) + "].lightColor", lightColors[i]);
		}
		shader.SetUniform3v("cameraPos", camera.position);
		shader.SetUniform1i("albedo_texture", 0);
		shader.SetUniform1i("normal_texture", 1);
		shader.SetUniform1i("roughness_texture", 2);
		shader.SetUniform1i("metallic_texture", 3);
		shader.SetUniform1i("ao_texture", 4);

		float roughness = 0.0f;
		float metallic = 0.0f;
		float offset = 15.0f;
		for (int i = -4; i < 4; i++)
		{
			roughness += 1.0f / 8.0f;
			roughness = glm::clamp(roughness, 0.05f, 1.0f);
			for (int j = -4; j < 4; j++)
			{
				metallic += 1.0 / 8.0f;
				metallic = glm::clamp(metallic, 0.05f, 1.0f);

				glm::mat4 temp_model = model;
				temp_model = glm::translate(temp_model, glm::vec3(i * offset, j * offset, 0));
				temp_model = glm::rotate(temp_model, glm::radians(-90.0f), glm::vec3(1.0f, 0, 0));
				shader.SetUniform1f("material.roughness", roughness);
				shader.SetUniform1f("material.metallic", metallic);
				shader.SetUniform3v("material.albedo", glm::vec3(0.6f, 0.6f, 0.6f));
				shader.SetUniformMat4("model", temp_model);
				ball.Draw(shader);
			}
			metallic = 0.0f;
		}

		camera.UpdateCameraPosition();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();

		auto end = glfwGetTime();
		double frame = 1.0f / (end - start);

		glfwSwapInterval(0);
		//frame <= 60 ? glfwSwapInterval(1) : glfwSwapInterval(1);

		glfwSetWindowTitle(window, (std::to_string(int(frame)) + "fps").data());
	}

	glfwTerminate();
	return 0;
}