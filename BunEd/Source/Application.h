#pragma once
#include "ImGui/ImGuiLayer.h"
#include <memory>

class Scene;

class Application
{
public:

	Application() = default;
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	bool Init();
	void Run();
	void Shutdown();

	void OnWindowResize(int width, int height);

	static Application& Get() { return s_Instance; }

private:

	static Application s_Instance;

	ImGuiLayer m_ImGuiLayer;

	std::shared_ptr<Scene> m_Scene = nullptr;

	float m_DeltaTime = 0.0f;
};
