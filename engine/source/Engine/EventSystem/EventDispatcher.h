#pragma once

#include "Event.h"
#include "EventBus.h"
#include <functional>

namespace engine::events
{
	template <typename TEvent>
	using EventCallback = std::function<bool(const TEvent&)>;

	class EventDispatcher : public common::NonCopyableObject
	{
	public:
		EventDispatcher(IN const Event& oEvent)
			: m_oEvent(oEvent)
		{
		}

		~EventDispatcher()
		{
		}

	public:
		template <class TEvent>
		bool Dispatch(IN EventCallback<TEvent> oEventCallback)
		{
			if (m_oEvent.GetEventType() == TEvent::GetStaticEventType())
			{
				oEventCallback(dynamic_cast<const TEvent&>(m_oEvent));
			}

			return true;
		}

	private:
		const Event& m_oEvent;
	};
}

#define BIND_EVENT(oFucntionReference) std::bind(&oFucntionReference, this, std::placeholders::_1)

#define SUBMIT_EVENT(Event) EventBus::GetInstance().Sumbit(Event);
#define REGISTER_EVENT_LISTENER(EventListener) EventBus::GetInstance().Register(EventListener);
#define UNREGISTER_EVENT_LISTENER() EventBus::GetInstance().UnRegister(this);
