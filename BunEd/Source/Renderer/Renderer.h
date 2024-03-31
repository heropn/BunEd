#pragma once
#include <memory>

class GameObject;
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

	void RenderScene(const std::shared_ptr<Scene>& scene);
	void RenderDepth(const std::shared_ptr<Scene>& scene);

	void RenderGameObject(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<Scene>& scene);

	static Renderer s_Instance;

	bool m_VisualizeDepth = false;
};
