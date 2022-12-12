#pragma once

#ifdef VP_PLATFORM_WINDOWS
	#ifdef VP_BUILD_DLL
		#define VIPER_API __declspec(dllexport)
	#else
		#define VIPER_API __declspec(dllimport)
	#endif
#else
	#error Viper only supports Windows
#endif