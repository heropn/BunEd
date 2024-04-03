#include "pch.h"
#include "ShadersManager.h"

ShadersManager ShadersManager::s_Instance;

void ShadersManager::Init()
{
	AddShader(ShaderType::ScreenRender, "Assets/Shaders/ScreenRender.vs", "Assets/Shaders/ScreenRender.ps");
	AddShader(ShaderType::MeshTex, "Assets/Shaders/MeshTex.vs", "Assets/Shaders/MeshTex.ps");
	AddShader(ShaderType::MeshColor, "Assets/Shaders/MeshColor.vs", "Assets/Shaders/MeshColor.ps");
	AddShader(ShaderType::DepthVisualizer, "Assets/Shaders/DepthVisualize.vs", "Assets/Shaders/DepthVisualize.ps");
	AddShader(ShaderType::StencilOutlinePP, "Assets/Shaders/StencilOutlinePP.vs", "Assets/Shaders/StencilOutlinePP.ps");
	AddShader(ShaderType::SkyBox, "Assets/Shaders/SkyBox.vs", "Assets/Shaders/SkyBox.ps");
	AddShader(ShaderType::EnviromentMapping, "Assets/Shaders/EnviromentMapping.vs", "Assets/Shaders/EnviromentMapping.ps");
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
