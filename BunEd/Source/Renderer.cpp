#include "pch.h"
#include "Renderer.h"
#include "glad/glad.h"

Renderer Renderer::s_Instance;

bool Renderer::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize OpenGL context");
		return false;
	}

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	return true;
}

void Renderer::Shutdown()
{
	
}

void Renderer::Clear()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(Window::Get().GetGLFWWindow());
}
