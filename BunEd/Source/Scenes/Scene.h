#pragma once
#include "Camera/Camera.h"
#include <memory>
#include "Renderer/ShadersManager.h"
#include "GameObjects/Lights.h"

class GameObject;
class DirectionalLight;

struct SceneLightData
{
	static constexpr int MAX_LIGHTS = 4;

	DirectionalLight m_DirLight;
	PointLight m_PointLights[MAX_LIGHTS];
	SpotLight m_SpotLights[MAX_LIGHTS];
};

class Scene
{
public:

	Scene();

	void Update(float deltaTime);

	inline const glm::vec3 GetCameraPos() const { return m_Camera.GetPosition(); }
	inline const glm::mat4x4 GetProjViewMatrix() const { return m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix(); }
	inline const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const { return m_GameObjects; }

	inline const SceneLightData& GetSceneLightData() const { return m_LightData; }

private:

	std::vector<std::shared_ptr<GameObject>> m_GameObjects;

	SceneLightData m_LightData;
	Camera m_Camera;
};
