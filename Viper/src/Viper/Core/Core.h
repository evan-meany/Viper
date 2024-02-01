#pragma once

#include <memory>

#ifdef VP_PLATFORM_WINDOWS
#if VP_DYNAMIC_LINK
	#ifdef VP_BUILD_DLL
		#define VIPER_API __declspec(dllexport)
	#else
		#define VIPER_API __declspec(dllimport)
	#endif
#else
	#define VIPER_API
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

namespace Viper {
	// In the future this will allow the removal of an atomic reference counter
	// that adds overhead for thread safety
	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T>
	using Unique = std::unique_ptr<T>;
}
