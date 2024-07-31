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
IncludeDir["spdlog"] = "JourneyEngine/vendor/spdlog/include"
IncludeDir["GLFW"] = "JourneyEngine/vendor/GLFW/include"
IncludeDir["GLAD"] = "JourneyEngine/vendor/GLAD/include"
IncludeDir["glm"] = "JourneyEngine/vendor/glm"
IncludeDir["imgui"] = "JourneyEngine/vendor/imgui/"
IncludeDir["EASTL"] = "JourneyEngine/vendor/eastl/include"
IncludeDir["EASTLBase"] = "JourneyEngine/vendor/eastl/test/packages/EABase/include/Common"

group "Dependencies"
	-- For window managment
	include "JourneyEngine/vendor/GLFW"
	-- For modern OpenGL
	include "JourneyEngine/vendor/GLAD"
	include "JourneyEngine/vendor/imgui/"
group ""

project "JourneyEngine"
	location "JourneyEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.EASTL}",
		"%{IncludeDir.EASTLBase}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{prj.name}/src"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
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

	filter "configurations:Debug"
		defines "JNY_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "JNY_RELEASE"
		symbols "on"
		optimize "on"
		runtime "Release"

	filter "configurations:Distribution"
		defines "JNY_DISTR"
		symbols "on"
		optimize "on"
		runtime "Release"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.EASTL}",
		"%{IncludeDir.EASTLBase}",
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
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "JNY_RELEASE"
		symbols "on"
		optimize "on"
		runtime "Release"

	filter "configurations:Distribution"
		defines "JNY_DISTR"
		symbols "on"
		optimize "on"
		runtime "Release"
