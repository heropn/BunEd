workspace "BunEd"
	architecture "x64"
	startproject "BunEd"
	configurations
	{
		"Debug",
		"Release",
	}
	
group "Dependencies"
	include "ThirdParty/_GLFW"
	include "ThirdParty/_GLAD"
	include "ThirdParty/imgui"
group ""

project "BunEd"
	location "BunEd"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"
	
	targetdir ("bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	objdir ("bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	
	includedirs
	{
		"BunEd/Source",
		"ThirdParty",
		"ThirdParty/_GLAD/include",
		"ThirdParty/_GLFW/include",
		"ThirdParty/_glm",
		"ThirdParty/imgui"
	}
	
	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}
	
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"ThirdParty/_glm/util/glm.natvis"
	}
	
	pchheader "pch.h"
	pchsource "%{prj.name}/Source/pch.cpp"
	
	filter "system:windows"
	cppdialect "C++17"
	systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"