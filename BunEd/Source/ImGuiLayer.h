#pragma once

class ImGuiLayer
{
public:

	ImGuiLayer() = default;
	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(const ImGuiLayer&) = delete;

	bool Init();
	void Shutdown();

	void PreRender();
	void Render();
	void PostRender();
};
