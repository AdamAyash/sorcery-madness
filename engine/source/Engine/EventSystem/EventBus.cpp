#include "EventBus.h"
#include "IEventListener.h"

namespace engine
{
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

		for (auto nIndex = 0; nIndex < m_oEventListenersMap.size(); ++nIndex)
		{
			const auto pEventListener = m_oEventListenersMap[nIndex];
			if (!pEventListener)
				continue;

			pEventListener->OnEvent(oEvent);
		}
	}
}