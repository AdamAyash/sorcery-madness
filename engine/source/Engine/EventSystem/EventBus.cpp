#include "EventBus.h"
#include "IEventListener.h"

namespace engine::events
{
	EventBus::EventBus()
	{
	}

	EventBus::~EventBus()
	{
		m_oEventListenersMap.clear();
	}

	EventBus &EventBus::GetInstance()
	{
		static EventBus s_oEventBus;
		return s_oEventBus;
	}

	void EventBus::Register(IN IEventListener *pEventListener)
	{
		if (!pEventListener)
			return;

		if (m_oEventListenersMap.contains(pEventListener->GetEventListenerSubscriptionId()))
			return;

		m_oEventListenersMap.emplace(pEventListener->GetEventListenerSubscriptionId(), pEventListener);
	}

	void EventBus::UnRegister(IN IEventListener *pEventListener)
	{
		if (!pEventListener)
			return;

		m_oEventListenersMap.erase(pEventListener->GetEventListenerSubscriptionId());
	}

	void EventBus::Sumbit(IN const Event &oEvent)
	{
		for (const auto& [nId, pEventListener] : m_oEventListenersMap)
		{
			if (!pEventListener)
				continue;

			pEventListener->OnEvent(oEvent);
		}
	}
}