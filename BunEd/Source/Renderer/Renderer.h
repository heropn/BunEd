#pragma once
#include <memory>

class Scene;

class Renderer
{
public:

	Renderer(const Renderer&) = delete;
	Renderer operator=(const Renderer&) = delete;

	bool Init(int width, int height);
	void Shutdown();

	void Render(const std::shared_ptr<Scene>& scene);

	void Clear();
	void SwapBuffers();
	void ChangeViewportSize(int width, int height);

	static Renderer& Get() { return s_Instance; };

private:

	Renderer() = default;

	void RenderDepth(const std::shared_ptr<Scene>& scene);

	static Renderer s_Instance;

	bool m_VisualizeDepth = true;
};
