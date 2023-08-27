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

Camera camera{ glm::vec3(0, 0, 10.0f), 0.0f, 0.0f, glm::vec3(0, 1.0f, 0) };
constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
constexpr unsigned int SCREEN_WIDHT = 800, SCREEN_HEIGHT = 600;

void RenderQuad();
void RenderCube();

#pragma region Data
float quadVertices[] = {
	// positions        // texture Coords
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.speedZ = -0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.speedZ = 0.03f;
	}
	else
	{
		camera.speedZ = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.speedX = -0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.speedX = 0.03f;
	}
	else
	{
		camera.speedX = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.speedY = 0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.speedY = -0.03f;
	}
	else
	{
		camera.speedY = 0.0f;
	}
}
#pragma endregion Interaction

void Render(Shader& shader);

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */

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
	Shader g_shader{ "./Shaders/DeferredVolumen/g_buffer.vs", "./Shaders/DeferredVolumen/g_buffer.fs" };
	Shader lighting_shader{ "./Shaders/DeferredVolumen/deferred_shading.vs", "./Shaders/DeferredVolumen/deferred_shading.fs" };
	Shader lightBox_shader{ "./Shaders/DeferredVolumen/light_box.vs", "./Shaders/DeferredVolumen/light_box.fs" };
#pragma endregion Shaders

	Model backpack{ "./Models/Backpack/backpack.obj" ,true };
	std::vector<glm::vec3> object_positions = {
		glm::vec3(-3.0,  -0.5, -3.0),
		glm::vec3(0.0,  -0.5, -3.0),
		glm::vec3(3.0,  -0.5, -3.0),
		glm::vec3(-3.0,  -0.5,  0.0),
		glm::vec3(0.0,  -0.5,  0.0),
		glm::vec3(3.0,  -0.5,  0.0),
		glm::vec3(-3.0,  -0.5,  3.0),
		glm::vec3(0.0,  -0.5,  3.0),
		glm::vec3(3.0,  -0.5,  3.0)
	};

#pragma region Framebuffer
	unsigned int g_fbo;
	glGenFramebuffers(1, &g_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
	//	position color buffer
	unsigned int position_texture;
	glGenTextures(1, &position_texture);
	glBindTexture(GL_TEXTURE_2D, position_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDHT, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_texture, 0);
	//	normal color buffer
	unsigned int normal_texture;
	glGenTextures(1, &normal_texture);
	glBindTexture(GL_TEXTURE_2D, normal_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDHT, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_texture, 0);
	//	color + specular color buffer
	unsigned int color_texture;
	glGenTextures(1, &color_texture);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDHT, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, color_texture, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDHT, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "g_fbo not complete! \n";
	}
#pragma endregion Framebuffer

#pragma region Lignting
	int number = 32;
	std::vector<glm::vec3> light_color;
	std::vector<glm::vec3> light_position;
	for (int i = 0; i < 32; i++)
	{
		float x = ((rand() % 100) / 100.0f) * 6.0 - 3.0;
		float y = ((rand() % 100) / 100.0f) * 6.0 - 4.0;
		float z = ((rand() % 100) / 100.0f) * 6.0 - 3.0;
		light_position.push_back(glm::vec3(x, y, z));

		float r = ((rand() % 100) / 200.0f) + 0.5f * (rand() % 10);
		float g = ((rand() % 100) / 200.0f) + 0.5f * (rand() % 10);
		float b = ((rand() % 100) / 200.0f) + 0.5f * (rand() % 10);
		light_color.push_back(glm::vec3(r, g, b));
	}
#pragma endregion Lighting

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		auto start = glfwGetTime();
		ProcessInput(window);

		glEnable(GL_DEPTH_TEST);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		//	geometry buffer
		glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		g_shader.Bind();
		g_shader.SetUniformMat4("view", view);
		g_shader.SetUniformMat4("projection", projection);
		for (unsigned int i = 0; i < object_positions.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, object_positions[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			g_shader.SetUniformMat4("model", model);
			backpack.Draw(g_shader);
		}

		//	lighting pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		lighting_shader.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, position_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_texture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, color_texture);
		lighting_shader.SetUniform1i("gPosition", 0);
		lighting_shader.SetUniform1i("gNormal", 1);
		lighting_shader.SetUniform1i("gColor", 2);
		lighting_shader.SetUniform3v("CameraPos", camera.position);
		for (unsigned int i = 0; i < light_position.size(); i++)
		{
			lighting_shader.SetUniform3v("lights[" + std::to_string(i) + "].lightPos", light_position[i]);
			lighting_shader.SetUniform3v("lights[" + std::to_string(i) + "].lightColor", light_color[i]);
			// update attenuation parameters and calculate radius
			const float linear = 0.7f;
			const float quadratic = 1.8f;
			float maxBrightness = std::fmaxf(std::fmaxf(light_color[i].r, light_color[i].g), light_color[i].b);
			float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (1.0 - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

			lighting_shader.SetUniform1f("lights[" + std::to_string(i) + "].linear", linear);
			lighting_shader.SetUniform1f("lights[" + std::to_string(i) + "].quadratic", quadratic);
			lighting_shader.SetUniform1f("lights[" + std::to_string(i) + "].radius", radius);
		}
		RenderQuad();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, g_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, SCREEN_WIDHT, SCREEN_HEIGHT, 0, 0, SCREEN_WIDHT, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		lightBox_shader.Bind();
		lightBox_shader.SetUniformMat4("projection", projection);
		lightBox_shader.SetUniformMat4("view", view);
		for (unsigned int i = 0; i < light_position.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, light_position[i]);
			model = glm::scale(model, glm::vec3(0.125f));
			lightBox_shader.SetUniformMat4("model", model);
			lightBox_shader.SetUniform3v("lightColor", light_color[i]);
			RenderCube();
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

void RenderCube()
{
	VertexBufferObject vbo{ vertices, sizeof(vertices) };
	vbo.Bind();

	VertexArrayObject vao{};
	vao.AddBuffer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	vao.AddBuffer(1, 3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
	vao.AddBuffer(2, 2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float));
	vao.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderQuad()
{
	VertexBufferObject vbo{ quadVertices, sizeof(quadVertices) };
	vbo.Bind();

	VertexArrayObject vao{};
	vao.AddBuffer(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
	vao.AddBuffer(1, 2, GL_FLOAT, false, 5 * sizeof(float), 3 * sizeof(float));
	vao.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}