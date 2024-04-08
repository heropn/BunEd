#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath = "");

	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& uniformName, float f1, float f2, float f3, float f4) const;
	void SetUniform4f(const std::string& uniformName, const glm::vec4& vec4) const;
	void SetUniform3f(const std::string& uniformName, float f1, float f2, float f3) const;
	void SetUniform3f(const std::string& uniformName, const glm::vec3& vec3) const;
	void SetUniform2f(const std::string& uniformName, float f1, float f2) const;
	void SetUniform1f(const std::string& uniformName, float f1) const;

	void SetUniform1i(const std::string& uniformName, int i1);

	void SetUniformMatrix4f(const std::string& uniformName, const glm::mat4& matrix) const;
	void SetUniformMatrix3f(const std::string& uniformName, const glm::mat3& matrix) const;

private:

	void CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath);
	void CompileShader(const uint32_t shaderID) const;

	std::string GetShaderSourceFromFile(const std::string& shaderFilePath) const;

	int GetUniformLocation(const std::string& uniformName) const;

	uint32_t m_ID = 0;
};
