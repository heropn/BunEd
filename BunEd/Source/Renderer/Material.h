#pragma once
#include <memory>

class Shader;

class Material
{
public:

	Material() = default;

	void SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }

	void BindShader();

private:

	std::shared_ptr<Shader> m_Shader = nullptr;
};
