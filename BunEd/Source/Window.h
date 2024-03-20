#pragma once
#include "GLFW\glfw3.h"


class Window
{
public:

	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;

	static Window& Get() { return s_Instance; }

	bool Init();
	void Shutdown();
	
	void PollEvents();

	bool RequestingClose();

	GLFWwindow* GetGLFWWindow() { return m_GLFWWindow; }

private:

	Window() = default;

	GLFWwindow* m_GLFWWindow = nullptr;

	static Window s_Instance;
};
