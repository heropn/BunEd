#pragma once
#include "glm/glm.hpp"

class DirectionalLight
{
public:

	DirectionalLight() : m_Direction(glm::vec3(0.0f, -1.0f, 0.0f)), m_Transform(glm::identity<glm::mat4x4>()) {}

	inline void SetDirection(const glm::vec3& dir) { m_Direction = glm::normalize(dir); }
	inline const glm::vec3& GetDirection() const { return m_Direction; }

	void SetTransform(const glm::mat4x4& transform) { m_Transform = transform; }
	const glm::mat4x4& GetTransform() const { return m_Transform; }

private:

	//glm::vec3 m_Color;
	glm::vec3 m_Direction;

	glm::mat4x4 m_Transform;
};
