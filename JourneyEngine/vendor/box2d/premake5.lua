project "box2d"
	kind "StaticLib"
	language "C"
	cdialect "C11"
	warnings "off"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.c",
		"include/**.h"
	}

	includedirs
	{
		"include",
		"src",
		"extern/simde"
	}

	filter "system:windows"
		buildoptions{ "/experimental:c11atomics" }
		systemversion "latest"

	filter "system:Linux"
		buildoptions { "-fPIE" }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		defines
		{
			"NDEBUG"
		}

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		symbols "off"
		defines
		{
			"NDEBUG"
		}
