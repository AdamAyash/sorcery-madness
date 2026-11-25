#pragma once

#include "NonCopyableObject.h"
#include <memory>
#include <utility>
#include "Logger/Logger.h"
#include "Timer.h"
#include "Assert.h"

#define InitializeMemory() SecureZeroMemory(this, sizeof(*this));

#define IN
#define OUT
#define OPTIONAL
#define PURE = 0;

#define BASE __super
#define ELEMENT_NOT_FOUND -1

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
	using Reference = std::shared_ptr<T>;
	template<class T, class ... Args>
	constexpr Reference<T> CreateReference(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using Clock = std::chrono::high_resolution_clock;
}