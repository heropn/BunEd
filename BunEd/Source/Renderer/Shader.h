#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& uniformName, float f1, float f2, float f3, float f4) const;
	void SetUniform4f(const std::string& uniformName, glm::vec4 vec4) const;
	void SetUniform3f(const std::string& uniformName, float f1, float f2, float f3) const;
	void SetUniform3f(const std::string& uniformName, glm::vec3 vec3) const;
	void SetUniform2f(const std::string& uniformName, float f1, float f2) const;
	void SetUniform1f(const std::string& uniformName, float f1) const;

	void SetUniformMatrix4f(const std::string& uniformName, glm::mat4 matrix) const;

private:

	void CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void CompileShader(const unsigned int shaderID) const;

	std::string GetShaderSourceFromFile(const std::string& shaderFilePath) const;

	int GetUniformLocation(const std::string& uniformName) const;

	unsigned int m_ID = 0;
};
