workspace "JourneyEngine"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Debug-AS",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"] = "JourneyEngine/vendor/spdlog/include"
IncludeDir["GLFW"] = "JourneyEngine/vendor/GLFW/include"
IncludeDir["GLAD"] = "JourneyEngine/vendor/GLAD/include"
IncludeDir["glm"] = "JourneyEngine/vendor/glm"
IncludeDir["imgui"] = "JourneyEngine/vendor/imgui/"
IncludeDir["entt"] = "JourneyEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "JourneyEngine/vendor/yaml-cpp/include/"
IncludeDir["ImGuizmo"] = "JourneyEngine/vendor/ImGuizmo/"

group "Dependencies"
	-- For window management
	include "JourneyEngine/vendor/GLFW"
	-- For modern OpenGL
	include "JourneyEngine/vendor/GLAD"
	include "JourneyEngine/vendor/imgui"
	include "JourneyEngine/vendor/yaml-cpp"
group ""

defines
{
	"YAML_CPP_STATIC_DEFINE",
	"IMGUI_DEFINE_MATH_OPERATORS"
}

-- Function to add compiler warnings
function AddCompilerWarnings()
	filter "system:linux"
		buildoptions
		{
			"-Wall",                           -- Enable most common warnings
			"-Wextra",                         -- Even more common errors will be checked
			"-Wpedantic",                      -- And moooore
			"-Werror",                         -- Treat warnings as errors
		}
end

-- Function to add common configurations
function AddCommonConfigurations()
	filter "configurations:Debug or configurations:Debug-AS"
		defines { "JNY_DEBUG", "JNY_ENABLE_ASSERTS" }
		symbols "on"
		runtime "Debug"

	filter { "system:windows", "configurations:Debug-AS" }
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		defines { "JNY_RELEASE", "JNY_ENABLE_ASSERTS" }
		symbols "on"
		optimize "on"
		runtime "Release"

	filter "configurations:Distribution"
		defines "JNY_DISTR"
		symbols "on"
		optimize "on"
		runtime "Release"
end

-- JourneyEngine Project
project "JourneyEngine"
	location "JourneyEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "jnypch.h"
	pchsource "JourneyEngine/src/jnypch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		
		"%{prj.name}/vendor/ImGuizmo/**.h",
		"%{prj.name}/vendor/ImGuizmo/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{prj.name}/src",
		"%{prj.name}/vendor/stb_image"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"yaml-cpp",
		"imgui"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"JNY_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "system:linux"
		links 
		{ 
			"Xrandr",
			"Xi",
			"GLEW",
			"GLU",
			"GL",
			"X11"
		}

	AddCompilerWarnings()
	AddCommonConfigurations()
	
-- Function to set up application projects
function SetupAppProject(name)
	project(name)
		location(name)
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"
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
			"%{IncludeDir.imgui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.yaml_cpp}",
			"%{IncludeDir.ImGuizmo}",
			"%{prj.name}/src",
			"JourneyEngine/src"
		}

		links
		{
			"JourneyEngine"
		}

		defines
		{
			"YAML_CPP_STATIC_DEFINE",
			"IMGUI_DEFINE_MATH_OPERATORS"
		}

		filter "system:windows"
			systemversion "latest"
			defines
			{
				"JNY_PLATFORM_WINDOWS"
			}

		filter "system:linux"
			links
			{
				"GLFW",
				"Glad",
				"ImGui",
				"Xrandr",
				"Xi",
				"GLU",
				"GL",
				"X11",
				"dl",
				"pthread",
				"stdc++fs"
			}

		AddCompilerWarnings()
		AddCommonConfigurations()
end

-- Sandbox Project
SetupAppProject("Sandbox")

-- Editor Project
SetupAppProject("Editor")