workspace "JourneyEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "JourneyEngine/vendor/GLFW/include"
IncludeDir["GLAD"] = "JourneyEngine/vendor/GLAD/include"
IncludeDir["imgui"] = "JourneyEngine/vendor/imgui/"
IncludeDir["EASTL"] = "JourneyEngine/vendor/eastl/include"
IncludeDir["EASTLBase"] = "JourneyEngine/vendor/eastl/test/packages/EABase/include/Common"

-- For window magment
include "JourneyEngine/vendor/GLFW"
-- For modern OpenGL
include "JourneyEngine/vendor/GLAD"
include "JourneyEngine/vendor/imgui/"

project "JourneyEngine"
	location "JourneyEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "jnypch.h"
	pchsource "JourneyEngine/src/jnypch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.EASTL}",
		"%{IncludeDir.EASTLBase}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{prj.name}/src"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"dwmapi.lib",
		"imgui"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		buildoptions { "/utf-8" }

		defines
		{
			"JNY_PLATFORM_WINDOWS",
			"JNY_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			"if not exist \"../bin/" .. outputdir .. "/Sandbox\" mkdir \"../bin/" .. outputdir .. "/Sandbox\"",
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "JNY_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "JNY_RELEASE"
		symbols "On"
		optimize "On"
		runtime "Release"

	filter "configurations:Distribution"
		defines "JNY_DISTR"
		symbols "Off"
		optimize "On"
		runtime "Release"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"JourneyEngine/vendor/spdlog/include",
		"%{IncludeDir.EASTL}",
		"%{IncludeDir.EASTLBase}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"JourneyEngine/src"
	}

	links
	{
		"JourneyEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		buildoptions { "/utf-8" }

		defines
		{
			"JNY_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines { "JNY_DEBUG", "JNY_ENABLE_ASSERTS" }
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "JNY_RELEASE"
		symbols "On"
		optimize "On"
		runtime "Release"

	filter "configurations:Distribution"
		defines "JNY_DISTR"
		symbols "Off"
		optimize "On"
		runtime "Release"
