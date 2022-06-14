#pragma once
#include <string>
#include <vector>

#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"

class Model {
public:
	Model(const std::string& path) {
		loadModel(path);
	}

	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	unsigned int generateTexture(const char* filepath, const std::string& dir);
};
