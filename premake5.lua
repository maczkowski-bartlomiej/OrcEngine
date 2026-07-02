GAME_NAME = "TestProject"
GAME_PATH = "TestProject/"
FMOD_DIR = "E:/CppLibs/FMOD/api"
MONO_DIR = "C:/Program Files/Mono"

workspace "OrcEngine"
	architecture "x64"
	startproject (GAME_NAME)
	configurations { "Debug", "Release" }

OUTPUT_DIR = "%{cfg.buildcfg}/%{cfg.system}_(%{cfg.architecture})"

ENGINE_NAME = "OrcEngine"
ENGINE_PATH = "OrcEngine/"
ENGINE_CPP_FILES_WORKSPACE = "Orc"

project "OrcEngine"
    location "OrcEngine"
    kind "StaticLib"
    staticruntime "Off"
	warnings "Everything"
	flags {	"Multiprocessorcompile" }

    language "C++"
    cppdialect "C++20"

    targetdir ("binaries/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("binaries-temp/" .. OUTPUT_DIR .. "/%{prj.name}")
	
	-- Warning supression:
	--[[
		  4006: 'symbol' already defined in the module; duplicate symbol ignored
		  4099: PDB (debug info) mismatch with .lib file
		  4098: defaultlib 'library' conflicts with other libraries
	]]
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
		"%{MONO_DIR}/include/mono-2.0",
    }

    libdirs
    {
        "%{FMOD_DIR}/core/lib/x64/",
        "%{FMOD_DIR}/studio/lib/x64/",
		"%{MONO_DIR}/lib",
    }
	
    filter "action:vs*"
		-- Compiler warning suppression:
		--[[
		  4100: unreferenced formal parameter (function argument is unused)
		  4820: structure padding added for alignment
		  4625: copy constructor was implicitly deleted
		  4626: assignment operator was implicitly deleted
		  5027: move assignment operator was implicitly defined as deleted
		  5038: data member 'member1' will be initialized after data member 'member2'
		  5045: spectre mitigation options ignored
		]]
		buildoptions { "/wd4100", "/wd4820", "/wd4625", "/wd4626", "/wd5027", "/wd5038", "/wd5045",  }
        externalanglebrackets "On" -- treat <> includes as external headers to reduce warnings
        externalwarnings "Off" -- disable warnings for external headers

    filter "configurations:Debug"
        defines { "ORC_DEBUG" }
        runtime "Debug"
        symbols "on"
		links { "fmodL_vc", "fmodstudioL_vc", "mono-2.0-sgen", "MonoPosixHelper", "libmono-static-sgen"}
		
    filter "configurations:Release"
        defines { "ORC_RELEASE" }
        runtime "Release"
        optimize "on"
		links { "fmod_vc", "fmodstudio_vc", "mono-2.0-sgen", "MonoPosixHelper", "libmono-static-sgen" }

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

	filter "configurations:Debug"
		defines "ORC_DEBUG"
		runtime "Debug"
		symbols "on"
		postbuildcommands {
			'{COPYFILE} "%{FMOD_DIR}/core/lib/x64/fmodL.dll" "%{cfg.targetdir}"',
			'{COPYFILE} "%{FMOD_DIR}/studio/lib/x64/fmodstudioL.dll" "%{cfg.targetdir}"',
			'{COPYFILE} "%{MONO_DIR}/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
			'{COPYFILE} "%{MONO_DIR}/bin/MonoPosixHelper.dll" "%{cfg.targetdir}"',
			'{COPYFILE} "%{MONO_DIR}/bin/libmono-btls-shared.dll" "%{cfg.targetdir}"',
			'{COPYFILE} "%{MONO_DIR}/lib/mono/4.8-api/mscorlib.dll" "%{cfg.targetdir}"',
        }
	
	filter "configurations:Release"
		defines "ORC_RELEASE"
		runtime "Release"
		optimize "on"
		postbuildcommands {
            "{COPYFILE} %{FMOD_DIR}/core/lib/x64/fmod.dll %{cfg.targetdir}",
            "{COPYFILE} %{FMOD_DIR}/studio/lib/x64/fmodstudio.dll %{cfg.targetdir}",
			"{COPYFILE} %{MONO_DIR}/bin/mono-2.0-sgen.dll %{cfg.targetdir}",
			"{COPYFILE} %{MONO_DIR}/bin/MonoPosixHelper.dll %{cfg.targetdir}",
			"{COPYFILE} %{MONO_DIR}/bin/libmono-btls-shared.dll %{cfg.targetdir}",
        }
