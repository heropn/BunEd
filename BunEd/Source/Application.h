#pragma once
#include "ImGuiLayer.h"

class Application
{
public:

	Application() = default;
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	bool Init();
	void Run();
	void Shutdown();

	static Application& Get() { return s_Instance; }

private:

	static Application s_Instance;

	ImGuiLayer m_ImGuiLayer;
};
