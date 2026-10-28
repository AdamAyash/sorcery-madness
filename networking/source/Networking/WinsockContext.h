#pragma once

#include <Common/Common.h>

namespace networking
{
	class WinsockContext final : public common::NonCopyableObject
	{
	public:
		WinsockContext();
		~WinsockContext();

	public:
		bool Initialize();
	};
}