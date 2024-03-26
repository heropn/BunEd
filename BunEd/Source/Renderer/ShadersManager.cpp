#include "pch.h"
#include "ShadersManager.h"

ShadersManager ShadersManager::s_Instance;

void ShadersManager::Init()
{
	AddShader(ShaderType::Default, "Assets/Shaders/defaultVS.glsl", "Assets/Shaders/defaultPS.glsl");
	AddShader(ShaderType::Mesh, "Assets/Shaders/MeshVS.glsl", "Assets/Shaders/MeshPS.glsl");
}

void ShadersManager::AddShader(const ShaderType type, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	m_ShadersMap[type] = std::make_shared<Shader>(vertexShaderFilePath, fragmentShaderFilePath);
}

std::shared_ptr<Shader> ShadersManager::GetShader(const ShaderType type)
{
	auto shaderIt = m_ShadersMap.find(type);
	if (shaderIt != m_ShadersMap.end())
	{
		return shaderIt->second;
	}

	return nullptr;
}
