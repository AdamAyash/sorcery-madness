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
			m_lEventListenerSubscriptionId = s_lEventListenerSubscriptionId++;
		}

	public:
		virtual ~IEventListener()
		{
		}

	public:
		virtual void OnEvent(const Event &oEvent) PURE;

	public:
		constexpr unsigned long GetEventListenerSubscriptionId() const
		{
			return m_lEventListenerSubscriptionId;
		}

	private:
		static unsigned long s_lEventListenerSubscriptionId;
		unsigned long m_lEventListenerSubscriptionId = 0;
	};
}