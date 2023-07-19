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

Camera camera{ glm::vec3(0, 10, 20.0f), 0.0f, 0.0f, glm::vec3(0, 1.0f, 0) };

#pragma region Data
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

float planeVertices[] = {
	// positions            // normals         // texcoords
	 25.0f, -0.6f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.6f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-25.0f, -0.6f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

	 25.0f, -0.6f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.6f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
	 25.0f, -0.6f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
};

float quadVertices[] = {
	// positions        // texture Coords
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */

	// 多采样必须在creatwindow前
	glfwWindowHint(GLFW_SAMPLES, 4);
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

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	VertexBufferObject plane_vbo{ planeVertices, sizeof(planeVertices) };
	plane_vbo.Bind();
	VertexArrayObject plane_vao{};
	plane_vao.Bind();
	plane_vao.AddBuffer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	plane_vao.AddBuffer(1, 3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
	plane_vao.AddBuffer(2, 2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float));

	VertexBufferObject cube_vbo{ vertices, sizeof(vertices) };
	cube_vbo.Bind();
	VertexArrayObject cube_vao{};
	cube_vao.Bind();
	cube_vao.AddBuffer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	cube_vao.AddBuffer(1, 3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
	cube_vao.AddBuffer(2, 2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float));

	VertexBufferObject quad_vbo{ quadVertices, sizeof(quadVertices) };
	quad_vbo.Bind();
	VertexArrayObject quad_vao{};
	quad_vao.Bind();
	quad_vao.AddBuffer(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
	quad_vao.AddBuffer(1, 2, GL_FLOAT, false, 5 * sizeof(float), 3 * sizeof(float));

	// shader
	Shader floor_shader{ "./Shaders/quad.vs", "./Shaders/quad.fs" };
	Shader simple_depth_shader{ "./Shaders/shadow_mapping_depth.vs", "./Shaders/shadow_mapping_depth.fs" };
	Shader shader{ "./Shaders/shadow_mapping_base.vs", "./Shaders/shadow_mapping_base.fs" };
	Shader light_shader{ "./Shaders/light.vs", "./Shaders/light.fs" };
	// texture
	Texture wood_texture{ "./Textures/wood.png", true };
	Texture floor_texutre{ "./Textures/concreteTexture.png", true };

	// model
	Model light{ "./Models/Planet/planet.obj" };

	// framebuffer
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	unsigned int depth_map_texture;
	glGenTextures(1, &depth_map_texture);
	glBindTexture(GL_TEXTURE_2D, depth_map_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*
		当一个片段完全位于Shadowmap之外(也就是位于光源椎体外)，它对应的Shadowmap纹理坐标就可能小于0或大于1，这就可能出现问题。使用GL_CLAMP_TO_BORDER后，
		超过0到1范围的纹理坐标，就会返回一个边界值。默认的边界值是(0, 0, 0, 0)。纹理如果包含了深度成分，那么它的第一个分量表示深度值。默认的边界值0在这里
		就会出问题，因为深度值0对应近平面，这就造成所有位于光椎体外的点都被认为在阴影中！所以，这里我们使用glTexParameterfv函数设置边界颜色为(1, 0, 0, 0)，
		从而使它们对应最大深度值，使渲染结果正确。
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		auto start = glfwGetTime();
		ProcessInput(window);

		glm::mat4 light_projection, light_view;
		glm::mat4 light_matrix;
		glm::vec3 light_pos = glm::vec3(-2.0f, 4.0f, -1.0f);
		light_pos.x += sin(glfwGetTime()) * 5;
		//light_pos.y += sin(glfwGetTime());
		light_pos.z += cos(glfwGetTime()) * 5;
		light_view = glm::lookAt(light_pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 25.0f);

		light_matrix = light_projection * light_view;

		//  render scene from light's point of view
		simple_depth_shader.Bind();
		simple_depth_shader.SetUniformMat4("LightSpaceMatrix", light_matrix);
		simple_depth_shader.SetUniform3f("LightPos", glm::value_ptr(light_pos));

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_DEPTH_BUFFER_BIT);

#pragma region Draw
		glm::mat4 floor_model = glm::mat4(1.0f);
		plane_vbo.Bind();
		plane_vao.Bind();
		simple_depth_shader.SetUniformMat4("model", floor_model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glm::mat4 cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(0.0f, 1.5f, 0.0));
		cube_model = glm::scale(cube_model, glm::vec3(0.5f));
		cube_vbo.Bind();
		cube_vao.Bind();
		simple_depth_shader.SetUniformMat4("model", cube_model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(2.0f, 0.0f, 1.0));
		cube_model = glm::scale(cube_model, glm::vec3(0.5f));
		cube_vbo.Bind();
		cube_vao.Bind();
		simple_depth_shader.SetUniformMat4("model", cube_model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(-1.0f, 0.0f, 2.0));
		cube_model = glm::rotate(cube_model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		cube_model = glm::scale(cube_model, glm::vec3(0.25));
		cube_vbo.Bind();
		cube_vao.Bind();
		simple_depth_shader.SetUniformMat4("model", cube_model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
#pragma endregion Draw
		//  render scene as normal using the generated depth/shadow map
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1024.0f / 1024.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

#pragma region 灯
		light_shader.Bind();
		light_shader.SetUniformMat4("view", view);
		light_shader.SetUniformMat4("projection", projection);

		glm::mat4 light_model = glm::mat4(1.0f);
		light_model = glm::translate(light_model, light_pos);
		light_model = glm::scale(light_model, glm::vec3(0.05f));
		light_shader.SetUniformMat4("model", light_model);
		light.Draw(light_shader);
#pragma endregion 灯

		shader.Bind();
		shader.SetUniformMat4("view", view);
		shader.SetUniformMat4("projection", projection);
		shader.SetUniformMat4("lightSpaceMatrix", light_matrix);
		shader.SetUniform3f("CameraPos", { camera.position.x, camera.position.y, camera.position.z });
		shader.SetUniform3f("LightPos", { light_pos.x, light_pos.y, light_pos.z });
		wood_texture.Usetexture(0);
		wood_texture.Bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depth_map_texture);
		shader.SetUniform1i("diffuse_texture", 0);
		shader.SetUniform1i("DepthMap", 1);

#pragma region Draw
		floor_model = glm::mat4(1.0f);
		plane_vbo.Bind();
		plane_vao.Bind();
		shader.SetUniformMat4("model", floor_model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(0.0f, 1.5f, 0.0));
		cube_model = glm::scale(cube_model, glm::vec3(0.5f));
		cube_vbo.Bind();
		cube_vao.Bind();
		shader.SetUniformMat4("model", cube_model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(2.0f, 0.0f, 1.0));
		cube_model = glm::scale(cube_model, glm::vec3(0.5f));
		cube_vbo.Bind();
		cube_vao.Bind();
		shader.SetUniformMat4("model", cube_model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(-1.0f, 0.0f, 2.0));
		cube_model = glm::rotate(cube_model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		cube_model = glm::scale(cube_model, glm::vec3(0.25));
		cube_vbo.Bind();
		cube_vao.Bind();
		shader.SetUniformMat4("model", cube_model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

#pragma endregion Draw

#pragma region 绘制阴影贴图
		//	render Depth map to quad for visual debugging
		glViewport(0, 0, 300, 300);
		quad_vbo.Bind();
		quad_vao.Bind();
		floor_shader.Bind();
		glActiveTexture(1);
		glBindTexture(GL_TEXTURE_2D, depth_map_texture);
		floor_shader.SetUniform1i("DepthMap", 1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#pragma endregion 绘制阴影贴图

		camera.UpdateCameraPosition();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		auto end = glfwGetTime();
		glfwSetWindowTitle(window, (std::to_string(1.0f / (end - start)) + "fps").data());
	}

	glfwTerminate();
	return 0;
}