#include "pch.h"
#include "Scene.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/DirectionalLight.h"
#include "Mesh/Mesh.h"
#include "Renderer/ShadersManager.h"

Scene::Scene()
{
	m_Camera.Init(glm::vec3(0.0f, 0.0f, 10.0f));

	std::shared_ptr<GameObject> gameObj = std::make_shared<GameObject>();
	gameObj->SetShader(ShadersManager::Get().GetShader(ShaderType::Mesh));
	gameObj->SetMesh(Mesh::CreateMesh("Assets/Models/Backpack/backpack.obj"));
	m_GameObjects.push_back(gameObj);

	m_DirLight = std::make_shared<DirectionalLight>();
	m_DirLight->SetTransform(glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(0.0f, 20.0f, 0.0f)));
}

void Scene::Update(float deltaTime)
{
	m_Camera.Update(deltaTime);
}
