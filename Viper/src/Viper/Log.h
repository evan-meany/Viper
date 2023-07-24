#pragma once

#include "vppch.h"

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Viper
{
	class VIPER_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define VP_CORE_TRACE(...)		::Viper::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VP_CORE_INFO(...)		::Viper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VP_CORE_WARN(...)		::Viper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VP_CORE_ERROR(...)		::Viper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VP_CORE_FATAL(...)		::Viper::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define VP_TRACE(...)			::Viper::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VP_INFO(...)			::Viper::Log::GetClientLogger()->info(__VA_ARGS__)
#define VP_WARN(...)			::Viper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VP_ERROR(...)			::Viper::Log::GetClientLogger()->error(__VA_ARGS__)
#define VP_FATAL(...)			::Viper::Log::GetClientLogger()->fatal(__VA_ARGS__)
