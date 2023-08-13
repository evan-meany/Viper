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

#ifdef VP_DEBUG
	#define VP_ENABLE_ASSERTS
#endif

#ifdef VP_ENABLE_ASSERTS
	#define VP_ASSERT(x, ...) { if(!(x)) { VP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define VP_CORE_ASSERT(x, ...) { if(!(x)) { VP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define VP_ASSERT(x, ...)
	#define VP_CORE_ASSERT(x, ...) 
#endif

#define BIT(x) (1 << x)

#define VP_BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
