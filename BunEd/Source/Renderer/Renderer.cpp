#include "pch.h"
#include "Renderer.h"
#include <format>
#include "glad/glad.h"
#include "Scenes/Scene.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Lights.h"
#include "Shader.h"
#include "Mesh/Mesh.h"
#include "Texture2D.h"

Renderer Renderer::s_Instance;

void GLAPIENTRY
MessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
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

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	ChangeViewportSize(width, height);

	return true;
}

void Renderer::Shutdown()
{
	
}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	const glm::mat4x4& PVMatrix = scene->GetProjViewMatrix();
	const glm::vec3& cameraPos = scene->GetCameraPos();
	const SceneLightData& lightData = scene->GetSceneLightData();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (const auto& gameObj : gameObjects)
	{
		std::shared_ptr<Shader> shader = gameObj->GetShader();

		shader->Bind();
		shader->SetUniformMatrix4f("u_Model", gameObj->GetTransform());
		shader->SetUniformMatrix4f("u_PV", PVMatrix);
		shader->SetUniform3f("u_EyePos", cameraPos);

		if (lightData.m_DirLight.isEnabled())
		{
			shader->SetUniform3f("u_DirLight.m_Ambient", lightData.m_DirLight.GetLightsColorConst().m_Ambient);
			shader->SetUniform3f("u_DirLight.m_Diffuse", lightData.m_DirLight.GetLightsColorConst().m_Diffuse);
			shader->SetUniform3f("u_DirLight.m_Specular", lightData.m_DirLight.GetLightsColorConst().m_Specular);
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
				shader->SetUniform3f(std::format("u_PointLights[{}].m_Ambient", pointLightsEnabledCount), pointLight.GetLightsColorConst().m_Ambient);
				shader->SetUniform3f(std::format("u_PointLights[{}].m_Diffuse", pointLightsEnabledCount), pointLight.GetLightsColorConst().m_Diffuse);
				shader->SetUniform3f(std::format("u_PointLights[{}].m_Specular", pointLightsEnabledCount), pointLight.GetLightsColorConst().m_Specular);
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
				shader->SetUniform3f(std::format("u_SpotLights[{}].m_Ambient", spotLightsEnabledCount), spotLight.GetLightsColorConst().m_Ambient);
				shader->SetUniform3f(std::format("u_SpotLights[{}].m_Diffuse", spotLightsEnabledCount), spotLight.GetLightsColorConst().m_Diffuse);
				shader->SetUniform3f(std::format("u_SpotLights[{}].m_Specular", spotLightsEnabledCount), spotLight.GetLightsColorConst().m_Specular);
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

			glDrawElements(GL_TRIANGLES, submesh->m_IndexBuffer->GetCount(), GL_UNSIGNED_SHORT, (const void*)0);
		}
	}
}

void Renderer::Clear()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(Window::Get().GetGLFWWindow());
}

void Renderer::ChangeViewportSize(int width, int height)
{
	glViewport(0, 0, width, height);
}
