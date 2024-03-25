#include "pch.h"
#include "Window.h"

Window Window::s_Instance;

bool Window::Init()
{
	if (!glfwInit())
	{
		return false;
	}

	m_GLFWWindow = glfwCreateWindow(m_Width, m_Height, "BunEd", NULL, NULL);
	if (!m_GLFWWindow)
	{
		return false;
	}

	glfwMakeContextCurrent(m_GLFWWindow);
	glfwSetWindowUserPointer(m_GLFWWindow, this);

	glfwGetCursorPos(m_GLFWWindow, reinterpret_cast<double*>(&m_LastMouseX), reinterpret_cast<double*>(&m_LastMouseY));
	glfwSwapInterval(1);

	BindToCallbacks();

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

void Window::BindToCallbacks()
{
	glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* glfwWindow, int width, int height)
		{
			Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			window->m_WindowSizeChangedDispatcher.Dispatch(width, height);
		});

	glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* glfwWindow)
		{
			Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			window->m_WindowClosedDispatcher.Dispatch();
		});

	glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			window->m_KeyStateChangedDispatcher.Dispatch(key, action, mods);
		});

	glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			window->m_MouseButtonStateChangedDispatcher.Dispatch(button, action, mods);
		});

	glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* glfwWindow, double xoffset, double yoffset)
		{
			Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			window->m_MouseScrollMovedDispatcher.Dispatch( static_cast<float>(xoffset), static_cast<float>(yoffset));
		});

	glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* glfwWindow, double xpos, double ypos)
		{
			Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			float fxpos = static_cast<float>(xpos);
			float fypos = static_cast<float>(ypos);

			window->m_CursorMovedDispatcher.Dispatch(window->m_LastMouseX, window->m_LastMouseY, fxpos, fypos);
			window->m_LastMouseX = fxpos;
			window->m_LastMouseY = fypos;
		});

	glfwSetErrorCallback([](int error_code, const char* description)
		{
			printf("ERROR [GLFW]: %d - %s\n", error_code, description);
		});
}
