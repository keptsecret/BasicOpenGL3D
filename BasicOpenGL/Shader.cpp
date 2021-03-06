#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexPath, vertex);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentPath, fragment);

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	GLint success = 0;
	char errorLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, errorLog);
		glDeleteShader(ID);

		std::cout << errorLog << std::endl;
		std::cout << "ERROR::Shader: Shader program failed to link!" << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::compileShader(const std::string &shader_path, const GLuint id) {
	// get shader code
	std::fstream file(shader_path);
	if (file.fail()) {
		perror(shader_path.c_str());
		std::cout << "ERROR::Shader: Failed to open " + shader_path << std::endl;
	}

	std::string file_content;
	std::string line;

	while (std::getline(file, line)) {
		file_content += line + "\n";
	}

	file.close();

	// compile shader from shader file contents
	const char *file_content_ptr = file_content.c_str();
	glShaderSource(id, 1, &file_content_ptr, nullptr);
	glCompileShader(id);

	GLint success = 0;
	char errorLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(id, 512, nullptr, errorLog);
		glDeleteShader(id);

		std::cout << errorLog << std::endl;
		std::cout << "ERROR::Shader: Shader " + shader_path + " failed to compile!" << std::endl;
	}
}
