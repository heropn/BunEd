#pragma once
#include "glm/glm.hpp"

struct LightColors
{
	glm::vec3 m_Ambient{};
	glm::vec3 m_Diffuse{};
	glm::vec3 m_Specular{};
};

class BaseLight
{
public:

	BaseLight() : m_DebugTransform(glm::identity<glm::mat4x4>()) {}

	const LightColors& GetLightsColorConst() const { return m_Color; }
	LightColors& GetLightsColor() { return m_Color; }

	void SetDebugTransform(const glm::mat4x4& transform) { m_DebugTransform = transform; }
	const glm::mat4x4& GetDebugTransform() const { return m_DebugTransform; }

	inline void SetIsEnabled(const bool isEnabled) { m_isEnabled = isEnabled; }
	inline bool isEnabled() const { return m_isEnabled; }

protected:

	glm::mat4x4 m_DebugTransform;
	LightColors m_Color;
	bool m_isEnabled = false;
};

class DirectionalLight : public BaseLight
{
public:

	DirectionalLight() : m_Direction(glm::vec3(0.0f, -1.0f, 0.0f)) {}

	inline void SetDirection(const glm::vec3& dir) { m_Direction = glm::normalize(dir); }
	inline const glm::vec3& GetDirection() const { return m_Direction; }

private:

	glm::vec3 m_Direction;
};

class PointLight : public BaseLight
{
public:

	PointLight() : m_Position(glm::zero<glm::vec3>()), m_Attenuation(glm::vec3(1.0f, 0.22f, 0.20f)) {}

	inline void SetPosition(const glm::vec3& pos) { m_Position = pos; }
	inline const glm::vec3& GetPosition() const { return m_Position; }

	inline void SetAttenuation(const glm::vec3& attenuation) { m_Attenuation = attenuation; }
	inline const glm::vec3& GetAttenuation() const { return m_Attenuation; }

private:

	glm::vec3 m_Position;
	glm::vec3 m_Attenuation;
};

class SpotLight : public BaseLight
{
public:

	SpotLight() : m_Position(glm::zero<glm::vec3>()), m_Direction(glm::vec3(0.0f, 0.0f, -1.0f)) {}

	inline void SetPosition(const glm::vec3& pos) { m_Position = pos; }
	inline const glm::vec3& GetPosition() const { return m_Position; }

	inline void SetDirection(const glm::vec3& dir) { m_Direction = glm::normalize(dir); }
	inline const glm::vec3& GetDirection() const { return m_Direction; }

	inline void SetInnerCutOffAngleDeg(const float deg) { m_InnerCutOffAngleCos = cosf(std::clamp(deg, 0.0f, 90.0f) * glm::pi<float>() / 180.0f); }
	inline float GetInnerCutOffAngleCos() const { return m_InnerCutOffAngleCos; }

	inline void SetOuterCutOffAngleDeg(const float deg) { m_OuterCutOffAngleCos = cosf(std::clamp(deg, 0.0f, 90.0f) * glm::pi<float>() / 180.0f); }
	inline float GetOuterCutOffAngleCos() const { return m_OuterCutOffAngleCos; }

private:

	glm::vec3 m_Position;
	glm::vec3 m_Direction;

	float m_InnerCutOffAngleCos = 0.9f;
	float m_OuterCutOffAngleCos = 0.8f;
};

/*
* Attentuation:
* Distance	Constant	Linear	Quadratic
* 7			1.0			0.7		1.8
* 13		1.0			0.35	0.44
* 20		1.0			0.22	0.20
* 32		1.0			0.14	0.07
* 50		1.0			0.09	0.032
* 65		1.0			0.07	0.017
* 100		1.0			0.045	0.0075
* 160		1.0			0.027	0.0028
* 200		1.0			0.022	0.0019
* 325		1.0			0.014	0.0007
* 600		1.0			0.007	0.0002
* 3250		1.0			0.0014	0.000007
*/