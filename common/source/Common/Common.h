#pragma once

#include "NonCopyableObject.h"
#include <memory>
#include <utility>
#include "Logger/Logger.h"
#include "Timer.h"
#include "Assert.h"
#include <chrono>

#define IN
#define OUT
#define OPTIONAL
#define PURE = 0

#define BASE __super

namespace common
{
	template<class T>
	using Scope = std::unique_ptr<T>;

	template<class T, class ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<class T>
	constexpr Scope<T> CreateScopeFromRaw(T* pRawPointer) noexcept
	{
		return Scope<T>(pRawPointer);
	}

	template<class T>
	using Reference = std::shared_ptr<T>;

	template<class T, class ... Args>
	constexpr Reference<T> CreateReference(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using Clock = std::chrono::steady_clock;
}