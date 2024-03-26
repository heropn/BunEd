#pragma once
#include <memory>
#include "glm/glm.hpp"

class Shader;
class Mesh;

class GameObject
{
public:

	GameObject();

	void SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }
	std::shared_ptr<Shader> GetShader() const { return m_Shader; }

	void SetMesh(const std::shared_ptr<Mesh>& mesh) { m_Mesh = mesh; }
	std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }

	void SetTransform(const glm::mat4x4& transform) { m_Transform = transform; }
	const glm::mat4x4& GetTransform() const { return m_Transform; }

private:

	glm::mat4x4 m_Transform;

	std::shared_ptr<Shader> m_Shader = nullptr;
	std::shared_ptr<Mesh> m_Mesh = nullptr;
};
