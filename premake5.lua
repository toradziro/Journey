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

group "Dependencies"
	-- For window management
	include "JourneyEngine/vendor/GLFW"
	-- For modern OpenGL
	include "JourneyEngine/vendor/GLAD"
	include "JourneyEngine/vendor/imgui"
group ""

-- Function to add compiler warnings
function AddCompilerWarnings()
	filter "system:windows"
		buildoptions
		{
			"/utf-8",
			"/w44265",                         -- Somewhat equivalent to -Wextra
			"/permissive-",                    -- Equivalent to -Wpedantic
			"/w44263",                         -- Equivalent to -Wctor-dtor-privacy
			"/w44862",                         -- Equivalent to -Wnon-virtual-dtor
			"/w44271",                         -- Equivalent to -Wold-style-cast
			"/w44459",                         -- Equivalent to -Woverloaded-virtual
			"/w44263",                         -- Equivalent to -Wsign-promo
			"/w44860",                         -- Equivalent to -Wduplicated-branches
			"/w44861",                         -- Equivalent to -Wduplicated-cond
			"/w44265",                         -- Equivalent to -Wfloat-equal
			"/w44267",                         -- Equivalent to -Wshadow=compatible-local
			"/w44266",                         -- Equivalent to -Wcast-qual
			"/w44265",                         -- Equivalent to -Wconversion
			"/w44268",                         -- Equivalent to -Wzero-as-null-pointer-constant
			"/w44266",                         -- Equivalent to -Wextra-semi
			"/w44472",                         -- Equivalent to -Wsign-conversion
			"/w44262",                         -- Equivalent to -Wlogical-op
		}

	filter "system:linux"
		buildoptions
		{
			"-Wall",                           -- Enable most common warnings
			"-Wextra",                         -- Even more common errors will be checked
			"-Wpedantic",                      -- And moooore
			"-Wctor-dtor-privacy",             -- Check if class with private constructor is used by any friends
			"-Wnon-virtual-dtor",              -- If you have virtual member funcs - don’t forget to have virtual ~
			"-Woverloaded-virtual",            -- We don’t overload virtual funcs, only overriding
			"-Wsign-promo",                    -- Overloading is not really accurate
			"-Wduplicated-cond",               -- Same, but with else if
			"-Wfloat-equal",                   -- Warning for float comparison
			"-Wshadow=compatible-local",       -- Shadowing warnings for local variables
			"-Wcast-qual",                     -- Warning for const qualification casts
			"-Wconversion",                    -- Warning for implicit type conversions
			"-Wextra-semi",                    -- Extra semicolon warnings
			"-Wlogical-op",                    -- Warning for logical operations
			"-Werror",                         -- Treat warnings as errors
			"-pedantic-errors"                 -- Treat pedantic warnings as errors
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
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.entt}",
		"%{prj.name}/src",
		"%{prj.name}/vendor/stb_image"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"imgui"
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
			"%{prj.name}/src",
			"JourneyEngine/src"
		}

		links
		{
			"JourneyEngine"
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