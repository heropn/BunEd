#pragma once

class Renderer
{
public:

	Renderer(const Renderer&) = delete;
	Renderer operator=(const Renderer&) = delete;

	bool Init(int width, int height);
	void Shutdown();

	void Render();

	void Clear();
	void SwapBuffers();
	void ChangeViewportSize(int width, int height);

	static Renderer& Get() { return s_Instance; };

private:

	Renderer() = default;

	static Renderer s_Instance;
};
