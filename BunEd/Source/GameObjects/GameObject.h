#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "Renderer/Material.h"

class Mesh;

class GameObject
{
public:

	GameObject();

	inline void SetMesh(const std::shared_ptr<Mesh>& mesh) { m_Mesh = mesh; }
	inline std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }

	inline void SetTransform(const glm::mat4x4& transform) { m_Transform = transform; }
	inline const glm::mat4x4& GetTransform() const { return m_Transform; }

	Material m_Material;

private:

	glm::mat4x4 m_Transform;

	std::shared_ptr<Mesh> m_Mesh = nullptr;
};
