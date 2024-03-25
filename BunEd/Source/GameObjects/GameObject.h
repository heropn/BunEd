#pragma once
#include <memory>

class Material;

class GameObject
{
public:

	GameObject() = default;

	void SetMaterial(const std::shared_ptr<Material>& mat) { m_Material = mat; }
	std::shared_ptr<Material> GetMaterial() const { return m_Material; }

private:

	std::shared_ptr<Material> m_Material = nullptr;
};
