#pragma once

#include "CallbackSink.h"

#include "Aryl/Core/Base.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Aryl
{
	enum class LogLevel : uint32_t
	{
		Trace = 0,
		Info,
		Warning,
		Error,
		Critical
	};

	class Log
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void SetLogLevel(spdlog::level::level_enum level);
		static void AddCallback(std::function<void(const LogCallbackData&)> callback);
		static void ClearCallbacks();

		inline static Ref<spdlog::logger> GetClientLogger() { return myClientLogger; }
		inline static Ref<spdlog::logger> GetCoreLogger() { return myCoreLogger; }

	private:
		inline static Ref<spdlog::logger> myClientLogger;
		inline static Ref<spdlog::logger> myCoreLogger;

		inline static Ref<CallbackSink<std::mutex>> myCallbackSink;
	};

	//Client logging macros
#define YL_TRACE(...)			::Aryl::Log::GetClientLogger()->trace(__VA_ARGS__)
#define YL_INFO(...)			::Aryl::Log::GetClientLogger()->info(__VA_ARGS__)
#define YL_WARN(...)			::Aryl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define YL_ERROR(...)			::Aryl::Log::GetClientLogger()->error(__VA_ARGS__)
#define YL_CRITICAL(...)		::Aryl::Log::GetClientLogger()->critical(__VA_ARGS__)

//Core logging macros
#define YL_CORE_TRACE(...)		::Aryl::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define YL_CORE_INFO(...)		::Aryl::Log::GetCoreLogger()->info(__VA_ARGS__)
#define YL_CORE_WARN(...)		::Aryl::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define YL_CORE_ERROR(...)		::Aryl::Log::GetCoreLogger()->error(__VA_ARGS__)
#define YL_CORE_CRITICAL(...)	::Aryl::Log::GetCoreLogger()->critical(__VA_ARGS__)
}
