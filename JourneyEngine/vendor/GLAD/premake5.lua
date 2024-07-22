project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/khrplatform/.h",
		"src/glad.c",
	}

	includedirs
	{
		"include"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "system:macosx"
		pic "On"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		buildoptions { "/MDd" }
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		buildoptions { "/MD" }
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		buildoptions { "/MD" }
		runtime "Release"
		optimize "speed"
        symbols "off"
