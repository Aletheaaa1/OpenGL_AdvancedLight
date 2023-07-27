#include "Mesh.h"

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
	:vertices(vertices), indices(indices), textures(textures)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (textures[i].type == "texture_diffuse")
		{
			//std::cout << textures[i].path << std::endl;
			glActiveTexture(GL_TEXTURE31);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			shader.SetUniform1i("texture_diffuse", 31);
		}
		else if (textures[i].type == "texture_specular")
		{
			glActiveTexture(GL_TEXTURE30);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			shader.SetUniform1i("texture_specular", 30);
		}
		else if (textures[i].type == "texture_reflection")
		{
			glActiveTexture(GL_TEXTURE29);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			shader.SetUniform1i("texture_reflection", 29);
		}
		else if (textures[i].type == "texture_normal")
		{
			glActiveTexture(GL_TEXTURE28);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			shader.SetUniform1i("texture_normal", 28);
		}
		else if (textures[i].type == "texture_height")
		{
			glActiveTexture(GL_TEXTURE27);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			shader.SetUniform1i("texture_height", 27);
		}
	}
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

unsigned int Mesh::GetVao()
{
	return vao;
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(MeshVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(MeshVertex), (void*)offsetof(MeshVertex, textCoords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Bitangent));

	glBindVertexArray(0);
}