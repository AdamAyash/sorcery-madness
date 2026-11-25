#pragma once

namespace common
{
	class NonCopyableObject
	{
	protected:
		NonCopyableObject() = default;

	public:
		~NonCopyableObject() = default;

	public:
		NonCopyableObject(const NonCopyableObject &) = delete;
		NonCopyableObject &operator=(const NonCopyableObject &) = delete;
	};
}
