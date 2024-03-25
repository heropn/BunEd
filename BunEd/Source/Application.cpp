#include "pch.h"
#include "Application.h"
#include "imgui.h"
#include "Renderer/Renderer.h"
#include "Scenes/Scene.h"
#include <chrono>

#include "Renderer/ShadersManager.h"
#include "Renderer/VertexArray.h"
#include "Mesh/Mesh.h"
#include "Renderer/Texture2D.h"

Application Application::s_Instance;

bool Application::Init()
{
	Window& window = Window::Get();
	if (!window.Init())
	{
		return false;
	}

	window.m_WindowSizeChangedDispatcher.Bind(CALLBACK_2(&Application::OnWindowResize, this));

	if (!Renderer::Get().Init(window.GetWidth(), window.GetHeight()))
	{
		return false;
	}

	if (!m_ImGuiLayer.Init())
	{
		return false;
	}

	ShadersManager::Get().Init();

	m_Scene = std::make_shared<Scene>();

	return true;
}

void Application::Run()
{
	bool show_demo_window = true;

	Renderer& renderer = Renderer::Get();
	Window& window = Window::Get();

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f
	};

	uint16_t indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexBuffer vbo(vertices, 5 * 4 * sizeof(float));
	IndexBuffer ibo(indicies, 6 * sizeof(uint16_t));
	VertexBufferLayout layout;
	layout.Push(3, GL_FLOAT, GL_FALSE);
	layout.Push(2, GL_FLOAT, GL_FALSE);

	std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>("Assets/Models/Spider/SpiderTex.jpg");

	VertexArray vao;
	vao.AddBuffer(vbo, layout, ibo);

	std::shared_ptr<Mesh> mesh = Mesh::CreateMesh("Assets/Models/Spider/spider.obj");

	while (!window.RequestingClose())
	{
		std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();

		window.PollEvents();

		m_ImGuiLayer.PreRender();

		m_Scene->Update(m_DeltaTime);

		renderer.Clear();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		m_ImGuiLayer.Render();

		{
			std::shared_ptr<Shader> shader = ShadersManager::Get().GetShader(ShaderType::Default);
			shader->Bind();
			shader->SetUniformMatrix4f("u_Model", glm::identity<glm::mat4>());
			shader->SetUniformMatrix4f("u_PV", m_Scene->GetProjViewMatrix());

			//vao.Bind();
			//tex->Bind(0);
			//glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_SHORT, (const void*)0);
			mesh->Draw();
		}

		renderer.Render(m_Scene);

		m_ImGuiLayer.PostRender();

		renderer.SwapBuffers();

		m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - time).count();
	}
}

void Application::Shutdown()
{
	m_ImGuiLayer.Shutdown();
	Window::Get().Shutdown();
}

void Application::OnWindowResize(int width, int height)
{
	Renderer::Get().ChangeViewportSize(width, height);
}
