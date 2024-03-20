#pragma once

class Renderer
{
public:

	Renderer(const Renderer&) = delete;
	Renderer operator=(const Renderer&) = delete;

	bool Init();
	void Shutdown();

	void Clear();
	void SwapBuffers();

	static Renderer& Get() { return s_Instance; };

private:

	Renderer() = default;

	static Renderer s_Instance;
};
