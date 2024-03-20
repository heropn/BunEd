project "GLAD"
	kind "StaticLib"
	language "C"
	systemversion "latest"
	staticruntime "Off"
	
	targetdir ("bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	objdir ("bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	
	includedirs
	{
		"include"
	}

	files
	{
		"include/glad/glad.h",
		"src/glad.c",
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
