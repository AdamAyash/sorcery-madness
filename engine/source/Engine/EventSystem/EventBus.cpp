#include "EventBus.h"
#include "IEventListener.h"

namespace engine
{
	unsigned long EventBus::s_lEventListenerSubscriptionId = 0;

	EventBus::EventBus()
	{
	}

	EventBus::~EventBus()
	{
	}

	EventBus &EventBus::GetInstance()
	{
		static EventBus s_oEventBus;
		return s_oEventBus;
	}

	void EventBus::Register(IN IEventListener *pEventListener)
	{
		m_oEventListeners.push_back(pEventListener);
	}

	void EventBus::UnRegister(IN IEventListener *pEventListener)
	{
		m_oEventListeners.
	}

	void EventBus::Sumbit(IN const Event &oEvent)
	{
		for (auto nIndex = 0; nIndex < m_oEventListeners.size(); ++nIndex)
		{
			const auto pEventListener = m_oEventListeners[nIndex];
			if (!pEventListener)
				continue;

			pEventListener->OnEvent(oEvent);
		}
	}
}