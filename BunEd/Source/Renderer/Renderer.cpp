#include "pch.h"
#include "Renderer.h"
#include <format>
#include "glad/glad.h"
#include "Camera/Camera.h"
#include "FrameBuffer.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Lights.h"
#include "Material.h"
#include "Mesh/Mesh.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Scenes/Scene.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

Renderer Renderer::s_Instance;

void GLAPIENTRY MessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
{
	if (type == GL_DEBUG_TYPE_OTHER)
	{
		return;
	}

	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

bool Renderer::Init(int width, int height)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize OpenGL context\n");
		return false;
	}

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);
#endif

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_STENCIL_TEST);

	ChangeViewportSize(width, height);
	CreateOffScreenBufferDrawData();
	CreateSkyBoxData();

	return true;
}

void Renderer::Shutdown()
{
	
}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	Clear();

	if (m_VisualizeDepth)
	{
		RenderDepth(scene);
		return;
	}

	m_OffScreenFrameBuffer->Bind();

	Clear();

	RenderScene(scene);

	// Skybox
	std::shared_ptr<Shader> skyBoxShader = ShadersManager::Get().GetShader(ShaderType::SkyBox);
	skyBoxShader->Bind();
	skyBoxShader->SetUniformMatrix4f("u_PV", scene->GetCamera().GetProjectionMatrix() * glm::mat4(glm::mat3(scene->GetCamera().GetViewMatrix())));

	m_SkyboxVertexArray->Bind();
	m_SkyBoxTexture->Bind(0);
	glDrawElements(GL_TRIANGLES, m_SkyboxIndexBuffer->GetCount(), GL_UNSIGNED_INT, (const void*)0);
	m_SkyBoxTexture->Unbind();

	m_OffScreenFrameBuffer->Unbind();

	// Post process
	ShadersManager::Get().GetShader(ShaderType::StencilOutlinePP)->Bind();
	m_ScreenVertexArray->Bind();
	m_OffScreenFrameBuffer->GetColorBufferTexture()->Bind(0);
	glBindTextureUnit(1, m_OffScreenFrameBuffer->GetStencilBufferTextureViewId());

	glDrawElements(GL_TRIANGLES, m_ScreenIndexBuffer->GetCount(), GL_UNSIGNED_INT, (const void*)0);

	m_OffScreenFrameBuffer->GetColorBufferTexture()->Unbind();
	glBindTextureUnit(1, 0);
}

void Renderer::Clear()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(Window::Get().GetGLFWWindow());
}

void Renderer::ChangeViewportSize(int width, int height)
{
	glViewport(0, 0, width, height);

	m_OffScreenFrameBuffer = FrameBuffer::CreateColorDepthStencilFrameBuffer(width, height);
}

void Renderer::RenderScene(const std::shared_ptr<Scene>& scene)
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xff);
	glStencilMask(0xff);

	const glm::mat4x4& PVMatrix = scene->GetProjViewMatrix();
	const glm::vec3& cameraPos = scene->GetCameraPos();
	const SceneLightData& lightData = scene->GetSceneLightData();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	std::vector<std::shared_ptr<GameObject>> translucentObjects;

	for (const auto& gameObj : gameObjects)
	{
		if (gameObj->m_Material.m_IsTranslucent)
		{
			translucentObjects.push_back(gameObj);
			continue;
		}

		RenderGameObject(gameObj, scene);
	}

	std::sort(translucentObjects.begin(), translucentObjects.end(), [&cameraPos](const std::shared_ptr<GameObject>& obj1, const std::shared_ptr<GameObject>& obj2)
		{
			glm::vec3 diff1 = glm::vec3(obj1->GetTransform()[3]) - cameraPos;
			float distSqr1 = glm::dot(diff1, diff1);

			glm::vec3 diff2 = glm::vec3(obj2->GetTransform()[3]) - cameraPos;
			float distSqr2 = glm::dot(diff2, diff2);

			return distSqr1 > distSqr2;
		});

	for (const auto& translucentGameObj : translucentObjects)
	{
		RenderGameObject(translucentGameObj, scene);
	}

	glDisable(GL_STENCIL_TEST);
}

void Renderer::RenderDepth(const std::shared_ptr<Scene>& scene)
{
	const glm::mat4x4& PVMatrix = scene->GetProjViewMatrix();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();
	std::shared_ptr<Shader> shader = ShadersManager::Get().GetShader(ShaderType::DepthVisualizer);

	for (const auto& gameObj : gameObjects)
	{
		shader->Bind();
		shader->SetUniformMatrix4f("u_Model", gameObj->GetTransform());
		shader->SetUniformMatrix4f("u_PV", PVMatrix);
		shader->SetUniform1f("u_NearPlane", scene->GetCamera().GetNearPlane());
		shader->SetUniform1f("u_FarPlane", scene->GetCamera().GetFarPlane());

		const std::vector<std::shared_ptr<SubMesh>>& subMeshes = gameObj->GetMesh()->GetSubMeshes();

		for (const auto& submesh : subMeshes)
		{
			submesh->m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, submesh->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, (const void*)0);
		}
	}
}

void Renderer::RenderGameObject(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<Scene>& scene)
{
	const glm::mat4x4& PVMatrix = scene->GetProjViewMatrix();
	const glm::vec3& cameraPos = scene->GetCameraPos();
	const SceneLightData& lightData = scene->GetSceneLightData();

	std::shared_ptr<Shader> shader = gameObj->m_Material.m_Shader;

	shader->Bind();
	shader->SetUniformMatrix4f("u_Model", gameObj->GetTransform());
	shader->SetUniformMatrix4f("u_PV", PVMatrix);
	shader->SetUniform3f("u_EyePos", cameraPos);
	
	m_SkyBoxTexture->Bind(3);

	if (lightData.m_DirLight.isEnabled())
	{
		shader->SetUniform3f("u_DirLight.m_Ambient", lightData.m_DirLight.m_Color.m_Ambient);
		shader->SetUniform3f("u_DirLight.m_Diffuse", lightData.m_DirLight.m_Color.m_Diffuse);
		shader->SetUniform3f("u_DirLight.m_Specular", lightData.m_DirLight.m_Color.m_Specular);
		shader->SetUniform3f("u_DirLight.m_Direction", lightData.m_DirLight.GetDirection());
	}
	else
	{
		shader->SetUniform3f("u_DirLight.m_Ambient", glm::zero<glm::vec3>());
		shader->SetUniform3f("u_DirLight.m_Diffuse", glm::zero<glm::vec3>());
		shader->SetUniform3f("u_DirLight.m_Specular", glm::zero<glm::vec3>());
	}

	int pointLightsEnabledCount = 0;
	for (int i = 0; i < SceneLightData::MAX_LIGHTS; i++)
	{
		const PointLight& pointLight = lightData.m_PointLights[i];
		if (pointLight.isEnabled())
		{
			shader->SetUniform3f(std::format("u_PointLights[{}].m_Ambient", pointLightsEnabledCount), pointLight.m_Color.m_Ambient);
			shader->SetUniform3f(std::format("u_PointLights[{}].m_Diffuse", pointLightsEnabledCount), pointLight.m_Color.m_Diffuse);
			shader->SetUniform3f(std::format("u_PointLights[{}].m_Specular", pointLightsEnabledCount), pointLight.m_Color.m_Specular);
			shader->SetUniform3f(std::format("u_PointLights[{}].m_Position", pointLightsEnabledCount), pointLight.GetPosition());
			shader->SetUniform3f(std::format("u_PointLights[{}].m_Attenuation", pointLightsEnabledCount), pointLight.GetAttenuation());
			pointLightsEnabledCount++;
		}
	}

	shader->SetUniform1i("u_PointLightsEnabledCount", pointLightsEnabledCount);

	int spotLightsEnabledCount = 0;
	for (int i = 0; i < SceneLightData::MAX_LIGHTS; i++)
	{
		const SpotLight& spotLight = lightData.m_SpotLights[i];
		if (spotLight.isEnabled())
		{
			shader->SetUniform3f(std::format("u_SpotLights[{}].m_Ambient", spotLightsEnabledCount), spotLight.m_Color.m_Ambient);
			shader->SetUniform3f(std::format("u_SpotLights[{}].m_Diffuse", spotLightsEnabledCount), spotLight.m_Color.m_Diffuse);
			shader->SetUniform3f(std::format("u_SpotLights[{}].m_Specular", spotLightsEnabledCount), spotLight.m_Color.m_Specular);
			shader->SetUniform3f(std::format("u_SpotLights[{}].m_Position", spotLightsEnabledCount), spotLight.GetPosition());
			shader->SetUniform3f(std::format("u_SpotLights[{}].m_Direction", spotLightsEnabledCount), spotLight.GetDirection());
			shader->SetUniform1f(std::format("u_SpotLights[{}].m_InnerCutOffAngleCos", spotLightsEnabledCount), spotLight.GetInnerCutOffAngleCos());
			shader->SetUniform1f(std::format("u_SpotLights[{}].m_OuterCutOffAngleCos", spotLightsEnabledCount), spotLight.GetOuterCutOffAngleCos());
			spotLightsEnabledCount++;
		}
	}

	shader->SetUniform1i("u_SpotLightsEnabledCount", spotLightsEnabledCount);

	const std::vector<std::shared_ptr<SubMesh>>& subMeshes = gameObj->GetMesh()->GetSubMeshes();

	for (const auto& submesh : subMeshes)
	{
		if (submesh->m_MaterialData.m_DiffuseTexture)
		{
			submesh->m_MaterialData.m_DiffuseTexture->Bind(0);
		}

		if (submesh->m_MaterialData.m_SpecularTexture)
		{
			submesh->m_MaterialData.m_SpecularTexture->Bind(1);
		}

		if (submesh->m_MaterialData.m_NormalsTexture)
		{
			submesh->m_MaterialData.m_NormalsTexture->Bind(2);
		}

		shader->SetUniform4f("u_Material.m_Color", submesh->m_MaterialData.m_Color);
		shader->SetUniform1f("u_Material.m_Shinieness", submesh->m_MaterialData.m_Shininess);

		submesh->m_VertexArray->Bind();

		glDrawElements(GL_TRIANGLES, submesh->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, (const void*)0);
	}
}

void Renderer::CreateOffScreenBufferDrawData()
{
	float vertexData[] = {
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	m_ScreenVertexBuffer = std::make_unique<VertexBuffer>(vertexData, 4 * 4 * sizeof(float));

	uint32_t indicesData[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_ScreenIndexBuffer = std::make_unique<IndexBuffer>(indicesData, 3 * 2 * sizeof(uint32_t));

	VertexBufferLayout layout;
	layout.Push(2, GL_FLOAT, false);
	layout.Push(2, GL_FLOAT, false);

	m_ScreenVertexArray = std::make_unique<VertexArray>();
	m_ScreenVertexArray->AddBuffer(*m_ScreenVertexBuffer, layout, *m_ScreenIndexBuffer);
}

void Renderer::CreateSkyBoxData()
{
	std::vector<std::string> skyboxFiles = {
		"Assets/Textures/Skybox/right.jpg",
		"Assets/Textures/Skybox/left.jpg",
		"Assets/Textures/Skybox/top.jpg",
		"Assets/Textures/Skybox/bottom.jpg",
		"Assets/Textures/Skybox/front.jpg",
		"Assets/Textures/Skybox/back.jpg",
	};

	m_SkyBoxTexture = std::make_unique<Texture3D>(skyboxFiles);

	float skyboxVertices[] = {
		-1.0f, -1.0f,  1.0f, // A 0
		 1.0f, -1.0f,  1.0f, // B 1
		 1.0f,  1.0f,  1.0f, // C 2
		-1.0f,  1.0f,  1.0f, // D 3

		-1.0f, -1.0f, -1.0f, // E 4
		 1.0f, -1.0f, -1.0f, // F 5
		 1.0f,  1.0f, -1.0f, // G 6
		-1.0f,  1.0f, -1.0f, // H 7
	};

	m_SkyboxVertexBuffer = std::make_unique<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));

	uint32_t indices[] = {
		// front
		2, 1, 0,
		0, 3, 2,

		// back
		7, 4, 5,
		5, 6, 7,

		// top
		6, 2, 3,
		3, 7, 6,

		// bottom
		1, 5, 4,
		4, 0, 1,

		// right
		6, 5, 1,
		1, 2, 6,

		// left
		3, 0, 4,
		4, 7, 3
	};

	m_SkyboxIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));

	VertexBufferLayout layout;
	layout.Push(3, GL_FLOAT, false);

	m_SkyboxVertexArray = std::make_unique<VertexArray>();
	m_SkyboxVertexArray->AddBuffer(*m_SkyboxVertexBuffer, layout, *m_SkyboxIndexBuffer);
}
