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
	cppdialect "C++20"
	systemversion "latest"
	
	targetdir ("bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	objdir ("bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	
	includedirs
	{
		"BunEd/Source",
		"ThirdParty",
		"ThirdParty/_GLAD/include",
		"ThirdParty/_GLFW/include",
		"ThirdParty/_glm",
		"ThirdParty/imgui",
		"ThirdParty/assimp_build/include"
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

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		
		links
		{
			"ThirdParty/assimp_build/Debug/assimp-vc142-mtd.lib"
		}
		
		postbuildcommands
		{
			"copy \"..\\ThirdParty\\assimp_build\\Debug\\assimp-vc142-mtd.dll\" \"..\\bin\\%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\\%{prj.name}\" /y"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		
		links
		{
			"ThirdParty/assimp_build/Release/assimp-vc142-mt.lib"
		}
		
		postbuildcommands
		{
			"copy \"..\\ThirdParty\\assimp_build\\Release\\assimp-vc142-mt.dll\" \"..\\bin\\%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\\%{prj.name}\" /y"
		}