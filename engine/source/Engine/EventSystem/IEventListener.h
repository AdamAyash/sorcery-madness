#pragma once

#include <Common/Common.h>
#include "Event.h"

namespace engine
{
	class IEventListener : public common::NonCopyableObject
	{
	protected:
		IEventListener()
		{
		}

	public:
		virtual ~IEventListener()
		{
		}

	public:
		virtual void OnEvent(const Event &oEvent) PURE;
	};
}