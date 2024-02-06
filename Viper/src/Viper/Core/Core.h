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

	/*
	
	//////////////////////////////
	// SHARED
	//////////////////////////////
	template<typename T>
	class Shared
	{
	private:
		std::shared_ptr<T> ptr;

	public:
		// Constructors, destructors, and assignment operators
		Shared() : ptr(nullptr) {}
		explicit Shared(T* rawPtr) : ptr(rawPtr) {}
		Shared(std::nullptr_t) : ptr(nullptr) {}
		Shared(const Shared& other) = default;
		Shared(Shared&& other) noexcept = default;
		Shared& operator=(const Shared& other) = default;
		Shared& operator=(Shared&& other) noexcept = default;
		~Shared() = default;

		// Reset methods
		template<typename... Args>
		void Reset(Args&&... args)
		{
			ptr.reset(new T(std::forward<Args>(args)...));
		}
		void Reset()
		{
			ptr.reset();
		}

		// Accessors
		T* Get() const { return ptr.get(); }
		T& operator*() const { return *ptr; }
		T* operator->() const { return ptr.get(); }
		Shared& operator=(std::nullptr_t)
		{
			ptr.reset();
			return *this;
		}

		// MakeShared factory method
		template<typename... Args>
		static Shared<T> MakeShared(Args&&... args)
		{
			return Shared<T>(new T(std::forward<Args>(args)...));
		}
	};

	//////////////////////////////
	// UNIQUE
	//////////////////////////////
	template<typename T>
	class Unique
	{
	private:
		std::unique_ptr<T> ptr;

	public:
		// Constructors, destructors, and assignment operators
		Unique() : ptr(nullptr) {}
		explicit Unique(T* rawPtr) : ptr(rawPtr) {}
		Unique(std::nullptr_t) : ptr(nullptr) {}
		Unique(const Unique& other) = delete;
		Unique(Unique&& other) noexcept = default;
		Unique& operator=(const Unique& other) = delete;
		Unique& operator=(Unique&& other) noexcept = default;
		~Unique() = default;

		// Reset methods
		template<typename... Args>
		void Reset(Args&&... args)
		{
			ptr.reset(new T(std::forward<Args>(args)...));
		}
		void Reset()
		{
			ptr.reset();
		}

		// Accessors
		T* Get() const { return ptr.get(); }
		T& operator*() const { return *ptr; }
		T* operator->() const { return ptr.get(); }
		Unique& operator=(std::nullptr_t)
		{
			ptr.reset();
			return *this;
		}

		// MakeUnique factory method
		template<typename... Args>
		static Unique<T> MakeUnique(Args&&... args)
		{
			return Unique<T>(new T(std::forward<Args>(args)...));
		}
	};

	*/
}
