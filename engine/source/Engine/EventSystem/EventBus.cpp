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

	EventBus& EventBus::GetInstance()
	{
		static EventBus oEventBus;
		return oEventBus;
	}

	void EventBus::Subscibe(IN IEventListener* pEventListener)
	{
		m_oEventListeners.push_back(pEventListener);
	}

	void EventBus::Sumbit(IN const Event& oEvent)
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