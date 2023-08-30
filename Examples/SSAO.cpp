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

Camera camera{ glm::vec3(0, 10.0f, 10.0f), 0.0f, 0.0f, glm::vec3(0, 1.0f, 0) };
constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
constexpr unsigned int SCREEN_WIDHT = 800, SCREEN_HEIGHT = 600;

void RenderQuad();
void RenderCube();

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
	std::string dir_path = "./Shaders/SSAO/";
	Shader g_shader{ dir_path + "g_buffer.vs", dir_path + "g_buffer.fs" };
	Shader ssao_shader{ dir_path + "ssao.vs", dir_path + "ssao.fs" };
	Shader blur_shader{ dir_path + "ssao.vs" , dir_path + "ssao_blur.fs" };
	Shader lighting_shader{ dir_path + "ssao.vs", dir_path + "ssao_lighting.fs" };
	Shader debug_shader{ dir_path + "debug.vs", dir_path + "debug.fs" };
#pragma endregion Shaders

#pragma region Texture
	Texture wall_diffuse_texture{ "./Textures/bricks2.jpg", true };
	Texture wall_normal_texture{ "./Textures/bricks2_normal.jpg", false };
#pragma endregion Texture

#pragma region Models
	Model backpack{ "./Models/Crisis/nanosuit.obj" ,false, false };
	//Model backpack{ "./Models/Mars/Mars.obj" ,false };
#pragma endregion Models

#pragma region Framebuffer
	//	geometry framebuffer
	// ----------------------
	unsigned int g_fbo;
	glGenFramebuffers(1, &g_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
	//	position color buffer
	unsigned int position_depth_texture;
	glGenTextures(1, &position_depth_texture);
	glBindTexture(GL_TEXTURE_2D, position_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDHT, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_depth_texture, 0);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//	ssao framebuffer
	//	-----------------
	unsigned int ssao_fbo;
	glGenFramebuffers(1, &ssao_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);
	unsigned int ssao_texture;
	glGenTextures(1, &ssao_texture);
	glBindTexture(GL_TEXTURE_2D, ssao_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCREEN_WIDHT, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ssao_fbo not complete! \n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//	blur framebuffer
	//	----------------
	unsigned int blur_fbo;
	glGenFramebuffers(1, &blur_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, blur_fbo);
	unsigned int blur_texture;
	glGenTextures(1, &blur_texture);
	glBindTexture(GL_TEXTURE_2D, blur_texture);
	//	对ssao贴图做模糊处理，因此只需要r通道
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCREEN_WIDHT, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur_texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "blur_fbo not complete! \n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion Framebuffer

#pragma region random kernel
	std::uniform_real_distribution<float> random_float(0.0, 1.0);
	std::default_random_engine generator;

	const int KERNEL_NUMBER = 64;
	std::vector<glm::vec3> ssao_kernels;
	for (size_t i = 0; i < KERNEL_NUMBER; i++)
	{
		float x = random_float(generator) * 2.0 - 1.0;
		float y = random_float(generator) * 2.0 - 1.0;
		float z = random_float(generator);
		glm::vec3 kernel{ x, y, z };
		kernel *= random_float(generator);
		kernel = glm::normalize(kernel);

		float scale = float(i) / 64.0;
		auto lerp = [](float x, float y, float k) -> float { return x + k * (y - x); };
		kernel *= lerp(0.1f, 1.0f, scale * scale);

		ssao_kernels.push_back(kernel);
	}

	//noise texture
	std::vector<glm::vec3> noise_colors;
	for (size_t i = 0; i < 16; i++)
	{
		float r = random_float(generator) * 2.0 - 1.0;
		float g = random_float(generator) * 2.0 - 1.0;
		float b = 0.0f;
		noise_colors.push_back(glm::vec3(r, g, b));
	}

	unsigned int noise_texture;
	glGenTextures(1, &noise_texture);
	glBindTexture(GL_TEXTURE_2D, noise_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, noise_colors.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#pragma endregion random kernel

#pragma region Lignting
	glm::vec3 lightPos = glm::vec3(2.0, 4.0, 2.0);
	glm::vec3 lightColor = glm::vec3(0.2, 0.2, 3.0);
#pragma endregion Lighting

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		auto start = glfwGetTime();
		ProcessInput(window);
		glfwSetWindowFocusCallback(window, ReLoad);
		if (reload_shader == true)
		{
			Shader g_shader1{ dir_path + "g_buffer.vs", dir_path + "g_buffer.fs" };
			Shader ssao_shader1{ dir_path + "ssao.vs", dir_path + "ssao.fs" };
			Shader debug_shader1{ dir_path + "debug.vs", dir_path + "debug.fs" };

			g_shader = g_shader1;
			ssao_shader = ssao_shader1;
			debug_shader = debug_shader1;
			reload_shader = false;
		}

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 50.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

#pragma region geometry fbo
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));

		wall_diffuse_texture.Usetexture(0);
		wall_diffuse_texture.Bind();
		wall_normal_texture.Usetexture(1);
		wall_normal_texture.Bind();

		g_shader.Bind();
		g_shader.SetUniform1i("texture_diffuse", 0);
		g_shader.SetUniform1i("texture_normal", 1);
		g_shader.SetUniformMat4("projection", projection);
		g_shader.SetUniformMat4("view", view);
		g_shader.SetUniformMat4("model", model);
		RenderQuad();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		model = glm::scale(model, glm::vec3(0.8f));
		g_shader.SetUniformMat4("model", model);
		backpack.Draw(g_shader);
#pragma endregion geometry fbo

#pragma region ssao fbo
		glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, position_depth_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_texture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, color_texture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, noise_texture);

		ssao_shader.Bind();
		ssao_shader.SetUniform1i("gPosition", 0);
		ssao_shader.SetUniform1i("gNormal", 1);
		ssao_shader.SetUniform1i("gColor", 2);
		ssao_shader.SetUniform1i("gNoise", 3);
		ssao_shader.SetUniformMat4("projection", projection);
		for (size_t i = 0; i < KERNEL_NUMBER; i++)
		{
			ssao_shader.SetUniform3v("kernels[" + std::to_string(i) + "]", ssao_kernels[i]);
		}

		RenderQuad();
#pragma endregion ssao fbo

#pragma region blur fbo
		glBindFramebuffer(GL_FRAMEBUFFER, blur_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssao_texture);

		blur_shader.Bind();
		blur_shader.SetUniform1i("texture_ssao", 0);
		RenderQuad();
#pragma endregion blur fbo

#pragma region screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, position_depth_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_texture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, color_texture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, blur_texture);

		lighting_shader.Bind();
		lighting_shader.SetUniform1i("gPosition", 0);
		lighting_shader.SetUniform1i("gNormal", 1);
		lighting_shader.SetUniform1i("gColor", 2);
		lighting_shader.SetUniform1i("ssao", 3);
		lighting_shader.SetUniform3v("cameraPos", camera.position);
		lighting_shader.SetUniform3v("lightPos", lightPos);
		lighting_shader.SetUniform3v("lightColor", lightColor);
		lighting_shader.SetUniformMat4("projection", projection);

		RenderQuad();

#pragma endregion screen

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
	vao.AddBuffer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	vao.AddBuffer(1, 3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
	vao.AddBuffer(2, 2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float));
	vao.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}