#pragma once

#include "NonCopyableObject.h"
#include <memory>
#include <utility>

namespace common
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Reference = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Reference<T> CreateReference(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}