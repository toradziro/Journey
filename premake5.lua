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
IncludeDir["EASTL"] = "JourneyEngine/vendor/eastl/include"
IncludeDir["EASTLBase"] = "JourneyEngine/vendor/eastl/test/packages/EABase/include/Common"

include "JourneyEngine/vendor/GLFW"

project "JourneyEngine"
    location "JourneyEngine"
    kind "SharedLib"
    language "C++"

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
        "%{prj.name}/src"
    }

    links
    {
        "GLFW",
        "opengl32.lib",
        "dwmapi.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        buildoptions { "/utf-8", "/MT" }

        defines
        {
            "JNY_PLATFORM_WINDOWS",
            "JNY_BUILD_DLL"
        }

        postbuildcommands
        {
            "if not exist \"../bin/" .. outputdir .. "/Sandbox\" mkdir \"../bin/" .. outputdir .. "/Sandbox\"",
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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

        buildoptions { "/utf-8", "/MT" }

        defines
        {
            "JNY_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "JNY_DEBUG", "JNY_ENABLE_ASSERTS" }
        symbols "On"

    filter "configurations:Release"
        defines "JNY_RELEASE"
        symbols "On"
        optimize "On"

    filter "configurations:Distribution"
        defines "JNY_DISTR"
        symbols "Off"
        optimize "On"
