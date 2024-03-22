#include "pch.h"
#include "Application.h"
#include "imgui.h"
#include "Renderer.h"
#include <chrono>

Application Application::s_Instance;

bool Application::Init()
{
	if (!Window::Get().Init())
	{
		return false;
	}

	Renderer::Get().Init();

	if (!m_ImGuiLayer.Init())
	{
		return false;
	}

	return true;
}

void Application::Run()
{
	bool show_demo_window = true;


	while (!Window::Get().RequestingClose())
	{
		std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();

		Window::Get().PollEvents();

		m_ImGuiLayer.PreRender();

		Renderer::Get().Clear();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		m_ImGuiLayer.Render();

		m_ImGuiLayer.PostRender();

		Renderer::Get().SwapBuffers();

		m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - time).count();
	}
}

void Application::Shutdown()
{
	m_ImGuiLayer.Shutdown();
	Window::Get().Shutdown();
}
