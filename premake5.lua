workspace "Server"
	architecture "x86_64"
	startproject "server"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vendor/glfw"
include "vendor/glad"

project "Server"
	location "server"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	pchheader "vkpch.h"
	pchsource "server/src/vkpch.cpp"

	targetdir ("bin")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"server/src/**.h",
		"server/src/**.cpp"
	}

	includedirs
	{
		"engine/src",
		"vendor/glad/include",
		"vendor/glfw/include",
		"vendor/imgui/include",
		"vendor/enet/include"
	}

	links 
	{ 
		"glfw",
		"glad"
	}

	defines
	{
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"VE_WIN"
		}

	filter "configurations:Debug"
		defines "VE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HVE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VE_DIST"
		runtime "Release"
		optimize "on"