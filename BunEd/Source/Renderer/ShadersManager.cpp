#include "pch.h"
#include "ShadersManager.h"

ShadersManager ShadersManager::s_Instance;

void ShadersManager::Init()
{
	AddShader(ShaderType::Default, "Assets/Shaders/default.vs", "Assets/Shaders/default.ps");
	AddShader(ShaderType::MeshTex, "Assets/Shaders/MeshTex.vs", "Assets/Shaders/MeshTex.ps");
	AddShader(ShaderType::MeshColor, "Assets/Shaders/MeshColor.vs", "Assets/Shaders/MeshColor.ps");
	AddShader(ShaderType::DepthVisualizer, "Assets/Shaders/DepthVisualize.vs", "Assets/Shaders/DepthVisualize.ps");
	AddShader(ShaderType::StencilOutline, "Assets/Shaders/StencilOutline.vs", "Assets/Shaders/StencilOutline.ps");
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
