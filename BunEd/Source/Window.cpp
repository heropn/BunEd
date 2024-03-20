#include "pch.h"
#include "Window.h"

Window Window::s_Instance;

bool Window::Init()
{
	if (!glfwInit())
	{
		return false;
	}

	m_GLFWWindow = glfwCreateWindow(640, 480, "BunEd", NULL, NULL);
	if (!m_GLFWWindow)
	{
		return false;
	}

	glfwMakeContextCurrent(m_GLFWWindow);

	return true;
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_GLFWWindow);
	glfwTerminate();
}

void Window::PollEvents()
{
	glfwPollEvents();
}

bool Window::RequestingClose()
{
	return glfwWindowShouldClose(m_GLFWWindow);
}
