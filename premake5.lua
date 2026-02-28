GAME_NAME = "TestProject"
GAME_PATH = "TestProject/"
FMOD_DIR = "E:/CppLibs/FMOD/api"

workspace "OrcEngine"
	architecture "x64"
	startproject (GAME_NAME)
	configurations { "Debug", "Release", "Distribution" }

OUTPUT_DIR = "%{cfg.buildcfg}/%{cfg.system}_(%{cfg.architecture})"

ENGINE_NAME = "OrcEngine"
ENGINE_PATH = "OrcEngine/"
ENGINE_CPP_FILES_WORKSPACE = "Orc"
	
project "OrcEngine"
    location "OrcEngine"
    kind "StaticLib"
    staticruntime "off"

    language "C++"
    cppdialect "C++20"

    targetdir ("binaries/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("binaries-temp/" .. OUTPUT_DIR .. "/%{prj.name}")
	
	linkoptions { "-IGNORE:4006,4099,4098" }

    files
    {
        "%{prj.name}/include/Orc/**.hpp",
        "%{prj.name}/include/Orc/**.cpp",
        "%{prj.name}/include/Orc/**.inl",
        "%{prj.name}/source/Orc/**.cpp",
        "%{prj.name}/source/Orc/**.inl",
        "%{prj.name}/include/dependencies/**.h",
        "%{prj.name}/include/dependencies/**.c",
        "%{prj.name}/include/dependencies/**.hpp",
        "%{prj.name}/include/dependencies/**.cpp",
    }    

    includedirs
    {
        "%{prj.name}/include/Orc",
        "%{prj.name}/include/dependencies",
        "%{FMOD_DIR}/core/inc",
        "%{FMOD_DIR}/studio/inc",
    }

    libdirs
    {
        "%{FMOD_DIR}/core/lib/x64/",
        "%{FMOD_DIR}/studio/lib/x64/",
    }

    filter "action:vs*"
        buildoptions "/wd4100 /wd4820"
        externalanglebrackets "On"
        externalwarnings "Off"

    filter "system:windows"
        systemversion "latest"
        defines { "ORC_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "ORC_DEBUG" }
        runtime "Debug"
        symbols "on"
		links { "fmodL_vc", "fmodstudioL_vc" }
		
    filter "configurations:Release"
        defines { "ORC_RELEASE" }
        runtime "Release"
        optimize "on"
		links { "fmod_vc", "fmodstudio_vc" }

    filter "configurations:Distribution"
        defines { "ORC_DISTRIBUTION" }
        runtime "Release"
        optimize "on"
		links { "fmod_vc", "fmodstudio_vc" }

    pchheader "%{prj.name}/include/Orc/OrcPch.hpp"
    pchsource "%{prj.name}/include/Orc/OrcPch.cpp"


project (GAME_NAME)
	location (GAME_NAME)
	kind "ConsoleApp"
	staticruntime "off"

	language "C++"
	cppdialect "C++20"
		
	targetdir ("binaries/" .. OUTPUT_DIR .. "/" .. GAME_PATH)
	objdir ("binaries-temp/" .. OUTPUT_DIR .. "/" .. GAME_PATH)

	files
	{
		GAME_PATH .. "include/**.hpp",
		GAME_PATH .. "source/**.cpp"
	}

	includedirs
	{
		ENGINE_PATH .. "include",
		GAME_PATH .. "include",
		"%{FMOD_DIR}/core/inc",
        "%{FMOD_DIR}/studio/inc",
	}

	links
	{
		ENGINE_NAME
	}

	filter "system:windows"
		systemversion "latest"

		defines { "ORC_PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		defines "ORC_DEBUG"
		runtime "Debug"
		symbols "on"
		postbuildcommands {
            "{COPYFILE} %{FMOD_DIR}/core/lib/x64/fmodL.dll %{cfg.targetdir}",
            "{COPYFILE} %{FMOD_DIR}/studio/lib/x64/fmodstudioL.dll %{cfg.targetdir}"
        }
	
	filter "configurations:Release"
		defines "ORC_RELEASE"
		runtime "Release"
		optimize "on"
		postbuildcommands {
            "{COPYFILE} %{FMOD_DIR}/core/lib/x64/fmod.dll %{cfg.targetdir}",
            "{COPYFILE} %{FMOD_DIR}/studio/lib/x64/fmodstudio.dll %{cfg.targetdir}"
        }

	filter "configurations:Distribution"
		defines "ORC_DISTRIBUTION"
		runtime "Release"
		optimize "on"
		postbuildcommands {
            "{COPYFILE} %{FMOD_DIR}/core/lib/x64/fmod.dll %{cfg.targetdir}",
            "{COPYFILE} %{FMOD_DIR}/studio/lib/x64/fmodstudio.dll %{cfg.targetdir}"
        }
	