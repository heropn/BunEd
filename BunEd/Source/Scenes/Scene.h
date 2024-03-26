#pragma once
#include "Camera/Camera.h"
#include <memory>

class GameObject;
class DirectionalLight;

class Scene
{
public:

	Scene();

	void Update(float deltaTime);

	inline glm::mat4x4 GetProjViewMatrix() const { return m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix(); }
	inline const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const { return m_GameObjects; }

private:

	std::vector<std::shared_ptr<GameObject>> m_GameObjects;

	std::shared_ptr<DirectionalLight> m_DirLight;

	Camera m_Camera;
};
