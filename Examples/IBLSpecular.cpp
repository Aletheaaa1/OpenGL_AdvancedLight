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

#pragma region Initial
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
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

#pragma endregion Initial

#pragma region Texture
	Texture hdr_texture{ "Textures/hdr/newport_loft.hdr" };
#pragma endregion Texture

#pragma region Shaders
	Shader skybox_shader{ "./Shaders/IBLSpecular/skybox.vs", "./Shaders/IBLSpecular/skybox.fs" };
	Shader hdr_shader{ "./Shaders/IBLSpecular/hdr_to_cubemap.vs", "./Shaders/IBLSpecular/hdr_to_cubemap.fs" };
	Shader PBR_shader{ "./Shaders/IBLSpecular/PBR.vs", "./Shaders/IBLSpecular/PBR.fs" };
	Shader light_shader{ "./Shaders/IBLSpecular/light.vs", "./Shaders/IBLSpecular/light.fs" };
	Shader con_shader{ "./Shaders/IBLSpecular/convolution.vs", "./Shaders/IBLSpecular/convolution.fs" };
	Shader prefilter_shader{ "./Shaders/IBLSpecular/prefilter.vs", "./Shaders/IBLSpecular/prefilter.fs" };
#pragma endregion Shaders

#pragma region Frambuffer
	//	attach to framebuffer
	glm::mat4 skybox_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 100.0f);
	std::vector<glm::mat4> skybox_views
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	unsigned int fbo;
	unsigned int rbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);

	unsigned int skybox_texture;
	glGenTextures(1, &skybox_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
#pragma endregion Frambuffer

#pragma region HDR -> Skybox
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 512, 512);

	hdr_texture.Usetexture(0);
	glBindTexture(GL_TEXTURE_2D, hdr_texture.GetTextureID());

	hdr_shader.Bind();
	hdr_shader.SetUniformMat4("projection", skybox_projection);
	for (unsigned int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox_texture, 0);
		hdr_shader.SetUniformMat4("view", skybox_views[i]);
		hdr_shader.SetUniform1i("skybox_texture", 0);

		RenderCube();
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDHT, SCREEN_HEIGHT);
#pragma endregion HDR -> Skybox

#pragma region Convolution
	unsigned int convolution_texture;
	glGenTextures(1, &convolution_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, convolution_texture);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindRenderbuffer(GL_FRAMEBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glActiveTexture(GL_TEXTURE0);

	con_shader.Bind();
	con_shader.SetUniform1i("skybox_texture", 0);
	con_shader.SetUniformMat4("projection", skybox_projection);
	for (unsigned int i = 0; i < 6; i++)
	{
		con_shader.SetUniformMat4("view", skybox_views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, convolution_texture, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDHT, SCREEN_HEIGHT);
#pragma endregion Convolution

#pragma region Prefilter
	unsigned int prefilter_texture;
	glGenTextures(1, &prefilter_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_texture);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);

	prefilter_shader.Bind();
	prefilter_shader.SetUniform1i("skybox_texture", 0);
	prefilter_shader.SetUniformMat4("projection", skybox_projection);

	constexpr int MIP_LEVEL = 5;
	for (unsigned int mip = 0; mip < MIP_LEVEL; mip++)
	{
		unsigned int mip_height = static_cast<unsigned int>(128 * pow(0.5, mip));
		unsigned int mip_width = static_cast<unsigned int>(128 * pow(0.5, mip));
		glViewport(0, 0, mip_height, mip_width);

		float roughness = float(mip) / float(MIP_LEVEL);
		prefilter_shader.SetUniform1f("roughness", roughness);
		for (unsigned int i = 0; i < 6; i++)
		{
			prefilter_shader.SetUniformMat4("view", skybox_views[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_texture, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDHT, SCREEN_HEIGHT);
#pragma endregion Prefilter

#pragma region Models
	Model light{ "./Models/planet/planet.obj" };
	Model mesh{ "./Models/ball/ball.obj" ,false };
#pragma endregion Models

#pragma region Light
	glm::vec3 lightPositions[] =
	{
			glm::vec3(-150.0f,  150.0f, 150.0f),
			glm::vec3(150.0f,  150.0f, 150.0f),
			glm::vec3(-150.0f, -150.0f, 150.0f),
			glm::vec3(150.0f, -150.0f, 150.0f),
	};
	glm::vec3 lightColors[] =
	{
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(3.0f, 3.0f, 3.0f)
	};
#pragma endregion Light

#pragma region Render loop
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		auto start = glfwGetTime();
		ProcessInput(window);
		glfwSetWindowFocusCallback(window, ReLoad);
		if (reload_shader == true)
		{
			//Shader new_shader{ "./Shaders/PBRTexture/pbr.vs", "./Shaders/PBRTexture/pbr.fs" };
			//shader = new_shader;
			reload_shader = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glDepthFunc(GL_LEQUAL);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f));
#pragma endregion Render loop

#pragma region PBR
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, convolution_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_texture);

		PBR_shader.Bind();
		PBR_shader.SetUniformMat4("projection", projection);
		PBR_shader.SetUniformMat4("view", view);
		PBR_shader.SetUniform3v("material.albedo", glm::vec3(0.85f));
		PBR_shader.SetUniform3v("cameraPos", camera.position);
		PBR_shader.SetUniform1i("IBL", 0);
		PBR_shader.SetUniform1i("Specular", 1);
		for (unsigned int i = 0; i < 4; i++)
		{
			PBR_shader.SetUniform3v("lights[" + std::to_string(i) + "].lightPos", lightPositions[i] + glm::vec3(100 * sin(glfwGetTime())));
			PBR_shader.SetUniform3v("lights[" + std::to_string(i) + "].lightColor", lightColors[i]);
		}

		float roughness = 0.0f;
		float metallic = 0.0f;
		float offset = 15.0f;
		for (int i = -4; i < 4; i++)
		{
			roughness += (1.0f / 8.0f);
			roughness = glm::clamp(roughness, 0.05f, 1.0f);
			for (int j = -4; j < 4; j++)
			{
				metallic += (1.0f / 8.0f);
				metallic = glm::clamp(metallic, 0.05f, 1.0f);

				glm::mat4 temp_model = model;
				temp_model = glm::translate(temp_model, glm::vec3(offset * i, offset * j, 0.0f));
				temp_model = glm::rotate(temp_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				PBR_shader.SetUniformMat4("model", temp_model);
				PBR_shader.SetUniform1f("material.roughness", roughness);
				PBR_shader.SetUniform1f("material.metallic", metallic);
				mesh.Draw(PBR_shader);
			}
			metallic = 0.0f;
		}

#pragma endregion PBR

#pragma region Lights
		light_shader.Bind();
		model = glm::scale(model, glm::vec3(0.2f));
		light_shader.SetUniformMat4("view", view);
		light_shader.SetUniformMat4("projection", projection);

		for (unsigned int i = 0; i < 4; i++)
		{
			using mat4 = glm::mat4;
			mat4 temp_model = glm::translate(model, lightPositions[i] + glm::vec3(100 * sin(glfwGetTime())));
			light_shader.SetUniformMat4("model", temp_model);
			light.Draw(light_shader);
		}
#pragma endregion Lights

#pragma region  hdr -> Skybox
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);

		skybox_shader.Bind();
		skybox_shader.SetUniformMat4("projection", projection);
		skybox_shader.SetUniformMat4("view", glm::mat3(view));
		skybox_shader.SetUniform1i("skybox_texutre", 0);

		RenderCube();
#pragma endregion hdr -> Skybox

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
	VertexBufferObject cube_vbo{ vertices, sizeof(vertices) };
	cube_vbo.Bind();
	VertexArrayObject cube_vao{};
	cube_vao.Bind();
	cube_vao.AddBuffer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	cube_vao.AddBuffer(1, 3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
	cube_vao.AddBuffer(2, 2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float));

	cube_vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}