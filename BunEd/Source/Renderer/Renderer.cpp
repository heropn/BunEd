#include "pch.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "Scenes/Scene.h"
#include "GameObjects/GameObject.h"
#include "Shader.h"
#include "Mesh/Mesh.h"

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
	glCullFace(GL_CCW);

	ChangeViewportSize(width, height);

	return true;
}

void Renderer::Shutdown()
{
	
}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	glm::mat4x4 PVMatrix = scene->GetProjViewMatrix();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (const auto& gameObj : gameObjects)
	{
		std::shared_ptr<Shader> shader = gameObj->GetShader();

		shader->Bind();
		shader->SetUniformMatrix4f("u_Model", gameObj->GetTransform());
		shader->SetUniformMatrix4f("u_PV", PVMatrix);

		gameObj->GetMesh()->Draw();
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
