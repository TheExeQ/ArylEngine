#pragma once

#include <memory>
#include <string>
#include <iostream>

#define BIT(X) (1 << (X))

#define YL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#ifdef YL_DEBUG
#define YL_DEBUGBREAK() __debugbreak()
#define YL_ENABLE_DEBUG_ALLOCATIONS
#define YL_ENABLE_SHADER_DEBUG
#define YL_SHADER_PRINT
#define YL_PROFILE_GPU
#define YL_OPTIMIZE_ON
#define YL_OPTIMIZE_OFF

#else

#ifdef YL_DIST
#define YL_OPTIMIZE_OFF
#define YL_OPTIMIZE_ON
#endif

#ifdef YL_RELEASE
#define YL_ENABLE_SHADER_DEBUG
#define YL_PROFILE_GPU
#define YL_OPTIMIZE_OFF __pragma(optimize("", off));
#define YL_OPTIMIZE_ON __pragma(optimize("", on));
#endif

#define YL_DEBUGBREAK();
#endif

#define SAFE_RELEASE(x) \
if (x)					\
{						\
	x->Release();		\
	x = nullptr;		\
}	

#ifdef YL_ENABLE_ASSERTS
#define YL_ASSERT(x, ...) { if(!(x)) { YL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); YL_DEBUGBREAK(); } }
#define YL_CORE_ASSERT(x, ...) { if(!(x)) { YL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); YL_DEBUGBREAK(); } }
#else
#define YL_ASSERT(x, ...)
#define YL_CORE_ASSERT(x, ...)
#endif

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Weak = std::weak_ptr<T>;

