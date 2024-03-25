#include "pch.h"
#include "Scene.h"

Scene::Scene()
{
	m_Camera.Init(glm::vec3(0.0f, 0.0f, 10.0f));
}

void Scene::Update(float deltaTime)
{
	m_Camera.Update(deltaTime);
}
