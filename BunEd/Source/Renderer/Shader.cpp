#include "pch.h"
#include "Shader.h"
#include <sstream>
#include <fstream>

Shader::Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	CreateShaderProgram(vertexShaderFilePath, fragmentShaderFilePath);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Bind() const
{
	glUseProgram(m_ID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& uniformName, float f1, float f2, float f3, float f4) const
{
	glUniform4f(GetUniformLocation(uniformName), f1, f2, f3, f4);
}

void Shader::SetUniform4f(const std::string& uniformName, const glm::vec4& vec4) const
{
	glUniform4f(GetUniformLocation(uniformName), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetUniform3f(const std::string& uniformName, float f1, float f2, float f3) const
{
	glUniform3f(GetUniformLocation(uniformName), f1, f2, f3);
}

void Shader::SetUniform3f(const std::string& uniformName, const glm::vec3& vec3) const
{
	glUniform3f(GetUniformLocation(uniformName), vec3.x, vec3.y, vec3.z);
}

void Shader::SetUniform2f(const std::string& uniformName, float f1, float f2) const
{
	glUniform2f(GetUniformLocation(uniformName), f1, f2);
}

void Shader::SetUniform1f(const std::string& uniformName, float f1) const
{
	glUniform1f(GetUniformLocation(uniformName), f1);
}

void Shader::SetUniform1i(const std::string& uniformName, int i1)
{
	glUniform1i(GetUniformLocation(uniformName), i1);
}

void Shader::SetUniformMatrix4f(const std::string& uniformName, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	std::string vertexShaderSource = GetShaderSourceFromFile(vertexShaderFilePath);
	const char* cStr = vertexShaderSource.c_str();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cStr, nullptr);
	CompileShader(vertexShader);

	std::string fragmentShaderSource = GetShaderSourceFromFile(fragmentShaderFilePath);
	cStr = fragmentShaderSource.c_str();
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cStr, nullptr);
	CompileShader(fragmentShader);

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);

	glLinkProgram(m_ID);

	int isLinked = 0;
	glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		char info[1024];
		glGetProgramInfoLog(m_ID, 1024, nullptr, info);
		printf("Shader error: %s\n", info);
		__debugbreak();
	}

	glDetachShader(m_ID, vertexShader);
	glDetachShader(m_ID, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::CompileShader(const unsigned int shaderID) const
{
	glCompileShader(shaderID);

	int successful;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &successful);

	if (!successful)
	{
		char info[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, info);
		printf("Shader error: %s\n", info);
		__debugbreak();
	}
}

std::string Shader::GetShaderSourceFromFile(const std::string& shaderFilePath) const
{
	std::ifstream file(shaderFilePath);
	std::string line;
	std::stringstream ss;

	while (std::getline(file, line))
	{
		ss << line + '\n';
	}

	return ss.str();
}

int Shader::GetUniformLocation(const std::string& uniformName) const
{
	return glGetUniformLocation(m_ID, uniformName.c_str());
}
