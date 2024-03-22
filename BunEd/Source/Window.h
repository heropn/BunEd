#pragma once
#include "GLFW\glfw3.h"
#include "Events/Event.h"

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

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	GLFWwindow* GetGLFWWindow() { return m_GLFWWindow; }

	EventDispatcher<int, int> m_WindowSizeChangedDispatcher;
	EventDispatcher<> m_WindowClosedDispatcher;
	EventDispatcher<int, int, int> m_KeyStateChangedDispatcher;
	EventDispatcher<int, int, int> m_MouseButtonStateChangedDispatcher;
	EventDispatcher<float, float> m_MouseScrollMovedDispatcher;
	EventDispatcher<float, float, float, float> m_CursorMovedDispatcher;

private:

	Window() = default;

	void BindToCallbacks();

	static Window s_Instance;

	GLFWwindow* m_GLFWWindow = nullptr;

	int m_Width = 1280;
	int m_Height = 720;

	float m_LastMouseX = 0.0f;
	float m_LastMouseY = 0.0f;
};
