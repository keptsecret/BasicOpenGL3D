#pragma once
#include "Shader.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t);

	void draw(Shader& shader);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO, VBO, EBO;

	void setup();
};
