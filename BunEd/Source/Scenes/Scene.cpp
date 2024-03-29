#include "pch.h"
#include "Scene.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Lights.h"
#include "Mesh/Mesh.h"

/*
todo next:
- Visualize position of lights
*/

Scene::Scene()
{
	m_Camera.Init(glm::vec3(0.0f, 0.0f, 10.0f));

	std::shared_ptr<GameObject> gameObj = std::make_shared<GameObject>();
	gameObj->SetShader(ShadersManager::Get().GetShader(ShaderType::MeshColor));
	gameObj->SetMesh(Mesh::CreateMesh("Assets/Models/Bunny/bunny.obj"));
	gameObj->SetTransform(glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(10.0f)));
	m_GameObjects.push_back(gameObj);

	m_LightData.m_DirLight.SetIsEnabled(true);
	m_LightData.m_DirLight.SetDebugTransform(glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(0.0f, 10.0f, 0.0f)));
	m_LightData.m_DirLight.SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	m_LightData.m_DirLight.GetLightsColor().m_Ambient = glm::vec3(0.1f);
	m_LightData.m_DirLight.GetLightsColor().m_Diffuse = glm::vec3(0.6f);
	m_LightData.m_DirLight.GetLightsColor().m_Specular = glm::vec3(1.0f);

	m_LightData.m_PointLights[0].SetIsEnabled(true);
	m_LightData.m_PointLights[0].SetPosition(glm::vec3(5.0f, 3.0f, 0.0f));
	m_LightData.m_PointLights[0].GetLightsColor().m_Ambient = glm::vec3(0.1f, 0.0f, 0.0f);
	m_LightData.m_PointLights[0].GetLightsColor().m_Diffuse = glm::vec3(0.8f, 0.0f, 0.0f);
	m_LightData.m_PointLights[0].GetLightsColor().m_Specular = glm::vec3(1.0f, 0.0f, 0.0f);

	m_LightData.m_SpotLights[0].SetIsEnabled(true);
	m_LightData.m_SpotLights[0].SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
	m_LightData.m_SpotLights[0].SetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	m_LightData.m_SpotLights[0].SetInnerCutOffAngleDeg(5.0f);
	m_LightData.m_SpotLights[0].SetOuterCutOffAngleDeg(15.0f);
	m_LightData.m_SpotLights[0].GetLightsColor().m_Ambient = glm::vec3(0.0f, 0.0f, 0.05f);
	m_LightData.m_SpotLights[0].GetLightsColor().m_Diffuse = glm::vec3(0.0f, 0.0f, 0.7f);
	m_LightData.m_SpotLights[0].GetLightsColor().m_Specular = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Scene::Update(float deltaTime)
{
	m_Camera.Update(deltaTime);
}
