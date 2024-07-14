workspace "JourneyEngine"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}
	
outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
		
project "JourneyEngine"
	location "JourneyEngine"
	kind "SharedLib"
	language "C++"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/fmt/include"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"JNY_PLATFORM_WINDOWS",
			"JNY_BUILD_DLL"
		}
		
		postbuildcommands
		{
			"if not exist \"../bin/" .. outputDir .. "/Sandbox\" mkdir \"../bin/" .. outputDir .. "/Sandbox\"",
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Sandbox")
		}
		
	filter "configurations:Debug"
		defines "JNY_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "JNY_RELEASE"
		symbols "On"
		optimize "On"
		
	filter "configurations:Distribution"
		defines "JNY_DISTR"
		symbols "Off"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"JourneyEngine/vendor/spdlog/include",
		"JourneyEngine/vendor/fmt/include",
		"JourneyEngine/src"
	}
	
	links
	{
		"JourneyEngine"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"JNY_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "JNY_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "JNY_RELEASE"
		symbols "On"
		optimize "On"
		
	filter "configurations:Distribution"
		defines "JNY_DISTR"
		symbols "Off"
		optimize "On"