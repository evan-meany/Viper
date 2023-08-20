workspace "Viper"
	architecture "x64"
	startproject "Sandbox"

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
IncludeDir["ImGui"] = "Viper/vendor/imgui"
IncludeDir["glm"] = "Viper/vendor/glm"

-- Include premake5.lua from vendors
include "Viper/vendor/GLFW"
include "Viper/vendor/Glad"
include "Viper/vendor/imgui"

project "Viper"
	location "Viper"
	kind "SharedLib"
	language "C++"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vppch.h"
	pchsource "Viper/src/vppch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"VP_PLATFORM_WINDOWS",
			"VP_BUILD_DLL"	 
		}

		postbuildcommands
		{

			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines 
		{
			"VP_DEBUG",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Release"
		defines "VP_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VP_DIST"
		runtime "Release"
		optimize "on"

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
		"Viper/vendor/spdlog/include",
		"Viper/vendor/glm/glm",
		"Viper/src"
	}

	links
	{
		"Viper"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"VP_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "VP_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VP_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VP_DIST"
		runtime "Release"
		optimize "on"