#pragma once
#include <memory>

class FrameBuffer;
class GameObject;
class IndexBuffer;
class Scene;
class SceneLightData;
class Texture3D;
class UniformBuffer;
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
	void RenderSceneNormals(const std::shared_ptr<Scene>& scene);
	void RenderDepth(const std::shared_ptr<Scene>& scene);

	void RenderGameObject(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<Scene>& scene);

	void CreateOffScreenBufferDrawData();
	void CreateSkyBoxData();

	void UpdateLightsUniformBuffer(const SceneLightData& lightData);

	static Renderer s_Instance;

	std::shared_ptr<FrameBuffer> m_OffScreenFrameBuffer = nullptr;

	std::unique_ptr<VertexBuffer> m_ScreenVertexBuffer = nullptr;
	std::unique_ptr<IndexBuffer> m_ScreenIndexBuffer = nullptr;
	std::unique_ptr<VertexArray> m_ScreenVertexArray = nullptr;

	std::unique_ptr<Texture3D> m_SkyBoxTexture = nullptr;
	std::unique_ptr<VertexBuffer> m_SkyboxVertexBuffer = nullptr;
	std::unique_ptr<IndexBuffer> m_SkyboxIndexBuffer = nullptr;
	std::unique_ptr<VertexArray> m_SkyboxVertexArray = nullptr;

	std::unique_ptr<UniformBuffer> m_LightsUniformBuffer = nullptr;

	bool m_VisualizeDepth = false;
	bool m_VisualizeNormals = false;
};
