#include "pch.h"
#include "Scene.h"
#include "GameObjects/GameObject.h"
#include "Renderer/ShadersManager.h"
#include "Mesh/Mesh.h"

Scene::Scene()
{
	m_Camera.Init(glm::vec3(0.0f, 0.0f, 10.0f));

	std::shared_ptr<GameObject> gameObj = std::make_shared<GameObject>();
	gameObj->SetShader(ShadersManager::Get().GetShader(ShaderType::Default));
	gameObj->SetMesh(Mesh::CreateMesh("Assets/Models/Spider/spider.obj"));
	m_GameObjects.push_back(gameObj);
}

void Scene::Update(float deltaTime)
{
	m_Camera.Update(deltaTime);
}
