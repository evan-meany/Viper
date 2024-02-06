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
IncludeDir["stb"] = "Viper/vendor/stb"

-- Include premake5.lua from vendors
include "Viper/vendor/GLFW"
include "Viper/vendor/Glad"
include "Viper/vendor/imgui"

project "Viper"
	location "Viper"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vppch.h"
	pchsource "Viper/src/vppch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb/**.h",
		"%{prj.name}/vendor/stb/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"VP_PLATFORM_WINDOWS",
			"VP_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"Viper/src"
	}

	links
	{
		"Viper"
	}

	filter "system:windows"
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
