#pragma once
#include <memory>

class FrameBuffer;
class GameObject;
class IndexBuffer;
class Scene;
class VertexArray;
class VertexBuffer;

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

	std::shared_ptr<FrameBuffer> m_OffScreenFrameBuffer = nullptr;

	std::unique_ptr<VertexBuffer> m_ScreenVertexBuffer = nullptr;
	std::unique_ptr<IndexBuffer> m_ScreenIndexBuffer = nullptr;
	std::unique_ptr<VertexArray> m_ScreenVertexArray = nullptr;

	bool m_VisualizeDepth = false;
};
