#pragma once
#include "glm/glm.hpp"

struct LightColors
{
	glm::vec3 m_Ambient = glm::zero<glm::vec3>();
	float pad1 = 0.0f;
	glm::vec3 m_Diffuse = glm::zero<glm::vec3>();
	float pad2 = 0.0f;
	glm::vec3 m_Specular = glm::zero<glm::vec3>();
	float pad3 = 0.0f;
};

class BaseLight
{
public:

	BaseLight() : m_DebugTransform(glm::identity<glm::mat4x4>()) {}

	void SetDebugTransform(const glm::mat4x4& transform) { m_DebugTransform = transform; }
	const glm::mat4x4& GetDebugTransform() const { return m_DebugTransform; }

	inline void SetIsEnabled(const bool isEnabled) { m_isEnabled = isEnabled; }
	inline bool isEnabled() const { return m_isEnabled; }

protected:

	glm::mat4x4 m_DebugTransform;
	bool m_isEnabled = false;
};

class DirectionalLight : public BaseLight
{
public:

	struct DirectionalLightData
	{
		LightColors m_Color;
		glm::vec3 m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		float pad1 = 0.0f;
	};

	DirectionalLight() = default;

	DirectionalLightData m_Data;
};

class PointLight : public BaseLight
{
public:

	struct PointLightData
	{
		LightColors m_Color;
		glm::vec3 m_Position = glm::zero<glm::vec3>();
		float pad1 = 0.0f;
		glm::vec3 m_Attenuation = glm::vec3(1.0f, 0.22f, 0.20f);
		float pad2 = 0.0f;
	};

	PointLight() = default;

	PointLightData m_Data;
};

class SpotLight : public BaseLight
{
public:

	struct SpotLightData
	{
		LightColors m_Color;
		glm::vec3 m_Position = glm::zero<glm::vec3>();
		float m_InnerCutOffAngleCos = 0.0f;
		glm::vec3 m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		float m_OuterCutOffAngleCos = 0.0f;
	};

	SpotLight() = default;

	inline void SetInnerCutOffAngleDeg(const float deg) { m_Data.m_InnerCutOffAngleCos = cosf(std::clamp(deg, 0.0f, 90.0f) * glm::pi<float>() / 180.0f); }

	inline void SetOuterCutOffAngleDeg(const float deg) { m_Data.m_OuterCutOffAngleCos = cosf(std::clamp(deg, 0.0f, 90.0f) * glm::pi<float>() / 180.0f); }

	SpotLightData m_Data;
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