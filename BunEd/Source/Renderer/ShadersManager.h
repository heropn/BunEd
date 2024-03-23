#pragma once
#include "Shader.h"
#include <unordered_map>
#include <memory>

enum class ShaderType
{
	Default,
};

class ShadersManager
{
public:

	ShadersManager(const ShadersManager& Other) = delete;
	ShadersManager(const ShadersManager&& Other) = delete;
	ShadersManager& operator=(const ShadersManager&& Other) = delete;
	ShadersManager& operator=(const ShadersManager& Other) = delete;

	void Init();

	void AddShader(const ShaderType type, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	std::shared_ptr<Shader> GetShader(const ShaderType type);

	static ShadersManager& Get() { return s_Instance; }

private:

	ShadersManager() = default;

	static ShadersManager s_Instance;

	std::unordered_map<ShaderType, std::shared_ptr<Shader>> m_ShadersMap;
};