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
