workspace "OrcEngine"
	architecture "x64"
	startproject "ShadowOfTheZombiePower"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}



outputDirectory = "%{cfg.buildcfg}/%{cfg.system}_(%{cfg.architecture})"

engineName = "OrcEngine"
enginePath = "OrcEngine/"
engineCppFilesWorkspace = "Orc"

gameName = "ShadowOfTheZombiePower"
gamePath = "ShadowOfTheZombiePower/"

project "OrcEngine"
	location "OrcEngine"

	kind "StaticLib"
	staticruntime "on"

	language "C++"
	cppdialect "C++20"

	targetdir ("binaries/" .. outputDirectory .. "/%{prj.name}")
	objdir ("binaries-temp/" .. outputDirectory .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.hpp",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/include/" .. engineCppFilesWorkspace,
		"%{prj.name}/source/" .. engineCppFilesWorkspace
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ORC_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ORC_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "ORC_RELEASE"
		runtime "release"
		optimize "on"

	filter "configurations:Distribution"
		defines "ORC_DISTRIBUTION"
		runtime "Release"
		optimize "on"


project "ShadowOfTheZombiePower"
	location "ShadowOfTheZombiePower"
	kind "ConsoleApp"
	staticruntime "on"

	language "C++"
	cppdialect "C++20"
		
	targetdir ("binaries/" .. outputDirectory .. "/" .. gamePath)
	objdir ("binaries-temp/" .. outputDirectory .. "/" .. gamePath)

	files
	{
		gamePath .. "include/**.hpp",
		gamePath .. "source/**.cpp"
	}

	includedirs
	{
		enginePath .. "include",
		gamePath .. "include"
	}

	links
	{
		engineName
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ORC_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ORC_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "ORC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "ORC_DISTRIBUTION"
		runtime "Release"
		optimize "on"
	