#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
	unsigned int ID;	//> ID of the shader program for this shader

	Shader(const std::string &vertexPath, const std::string &fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec3(const std::string& name, float x, float y, float z);
	void setVec3(const std::string& name, const glm::vec3& value);
	void setVec4(const std::string& name, float x, float y, float z, float w);
	void setVec4(const std::string& name, const glm::vec4& value);
	void setMat4(const std::string& name, const glm::mat4& value);

private:
	void compileShader(const std::string &shader_path, const GLuint id);
};
