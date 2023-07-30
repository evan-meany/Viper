workspace "Viper"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"] = "Viper/vendor/spdlog/include"
IncludeDir["GLFW"] = "Viper/vendor/GLFW/include"
IncludeDir["Glad"] = "Viper/vendor/Glad/include"

-- Include premake5.lua from vendors
include "Viper/vendor/GLFW"
include "Viper/vendor/Glad"

project "Viper"
	location "Viper"
	kind "SharedLib"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vppch.h"
	pchsource "Viper/src/vppch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"VP_PLATFORM_WINDOWS",
			"VP_BUILD_DLL"	 
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		buildoptions "/MDd"
		symbols "on"

		defines 
		{
			"VP_DEBUG",
			"VP_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Release"
		defines "VP_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "VP_DIST"
		buildoptions "/MD"
		optimize "on"

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
		"Viper/vendor/spdlog/include",
		"Viper/src"
	}

	links
	{
		"Viper"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"VP_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "VP_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "VP_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "VP_DIST"
		buildoptions "/MD"
		optimize "on"