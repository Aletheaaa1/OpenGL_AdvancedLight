#pragma once
#include "Mesh.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <vector>
class Model
{
public:
	bool flip;
	bool gamma;
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<MeshTexture> textures_loaded;
	Model(const std::string& file_path, bool gamma = false, bool flip = false);
	void Draw(Shader& shader);
private:
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};
