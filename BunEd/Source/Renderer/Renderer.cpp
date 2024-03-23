#include "pch.h"
#include "Renderer.h"
#include "glad/glad.h"

Renderer Renderer::s_Instance;

void GLAPIENTRY
MessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

bool Renderer::Init(int width, int height)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize OpenGL context\n");
		return false;
	}

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);
#endif

	ChangeViewportSize(width, height);

	return true;
}

void Renderer::Shutdown()
{
	
}

void Renderer::Render()
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

void Renderer::ChangeViewportSize(int width, int height)
{
	glViewport(0, 0, width, height);
}
