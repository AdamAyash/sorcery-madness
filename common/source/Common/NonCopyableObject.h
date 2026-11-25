#pragma once

namespace common
{
	class NonCopyableObject
	{
	protected:
		NonCopyableObject() = default;
		virtual ~NonCopyableObject() = default;

	public:
		NonCopyableObject(const NonCopyableObject &) = delete;
		NonCopyableObject &operator=(const NonCopyableObject &) = delete;
	};
}
