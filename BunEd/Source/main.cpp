#include "pch.h"
#include "Application.h"

int main(int argc, char* argv[])
{
	Application& app = Application::Get();

	if (app.Init())
	{
		app.Run();
	}

	app.Shutdown();
	return 0;
}