#pragma once

#include "Event.h"
#include "EventBus.h"
#include <functional>

namespace engine::events
{
	template<typename TEvent, typename = typename std::enable_if<std::is_base_of_v<Event, TEvent>>>
	using EventCallback = std::function<bool(const TEvent&)>;

	class EventDispatcher : public common::NonCopyableObject
	{
	public:
		EventDispatcher(IN Event& oEvent)
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
				m_oEvent.bIsHandled = oEventCallback(dynamic_cast<const TEvent&>(m_oEvent));
			}

			return true;
		}

	private:
		Event& m_oEvent;
	};
}

#define BIND_EVENT(oFucntionReference) std::bind(&oFucntionReference, this, std::placeholders::_1)

#define SUBMIT_EVENT(Event) EventBus::GetInstance().Sumbit(Event);
#define REGISTER_EVENT_LISTENER(EventListener) EventBus::GetInstance().Register(EventListener);
#define UNREGISTER_EVENT_LISTENER(EventListener) EventBus::GetInstance().UnRegister(EventListener);
