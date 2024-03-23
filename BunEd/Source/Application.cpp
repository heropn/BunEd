#include "pch.h"
#include "Application.h"
#include "imgui.h"
#include "Renderer/Renderer.h"
#include <chrono>

#include "Renderer/ShadersManager.h"
#include "Renderer/VertexArray.h"

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

	return true;
}

void Application::Run()
{
	bool show_demo_window = true;

	Renderer& renderer = Renderer::Get();
	Window& window = Window::Get();

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indicies[] = {
		0, 1, 2
	};

	VertexBuffer vbo(vertices, 6 * 3 * sizeof(float));
	IndexBuffer ibo(indicies, 3 * sizeof(unsigned int));
	VertexBufferLayout layout;
	layout.Push(3, GL_FLOAT, GL_FALSE);
	layout.Push(3, GL_FLOAT, GL_FALSE);

	VertexArray vao;
	vao.AddBuffer(vbo, layout, ibo);

	while (!window.RequestingClose())
	{
		std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();

		window.PollEvents();

		m_ImGuiLayer.PreRender();

		renderer.Clear();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		m_ImGuiLayer.Render();

		{
			std::shared_ptr<Shader> shader = ShadersManager::Get().GetShader(ShaderType::Default);
			shader->Bind();
			shader->SetUniformMatrix4f("u_Model", glm::identity<glm::mat4>());
			shader->SetUniformMatrix4f("u_PV", glm::identity<glm::mat4>());
			vao.Bind();

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		renderer.Render();

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
