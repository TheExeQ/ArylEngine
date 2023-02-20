#include "ylpch.h"
#include "Aryl/Log/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Aryl
{
	void Log::Initialize()
	{
		myCallbackSink = CreateRef<CallbackSink<std::mutex>>();

		spdlog::set_pattern("%^[%T] %n: %v%$");

		myClientLogger = spdlog::stdout_color_mt("APP");
		myClientLogger->sinks().emplace_back(myCallbackSink);
		myClientLogger->set_level(spdlog::level::trace);

		myCoreLogger = spdlog::stdout_color_mt("ARYL");
		myCoreLogger->sinks().emplace_back(myCallbackSink);
		myCoreLogger->set_level(spdlog::level::trace);
	}

	void Log::Shutdown()
	{
		myCallbackSink->ClearCallbacks();
		myCallbackSink = nullptr;
	}

	void Log::SetLogLevel(spdlog::level::level_enum level)
	{
		myCoreLogger->set_level(level);
		myClientLogger->set_level(level);
	}

	void Log::AddCallback(std::function<void(const LogCallbackData&)> callback)
	{
		myCallbackSink->AddCallback(callback);
	}

	void Log::ClearCallbacks()
	{
		myCallbackSink->ClearCallbacks();
	}
}
