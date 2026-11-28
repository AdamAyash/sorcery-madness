#pragma once

#include "Event.h"
#include "EventBus.h"
#include <functional>

namespace engine::events
{
	template<std::derived_from<Event> TEvent>
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
			static_assert(std::is_convertible_v<TEvent*, Event*>, "TEvent must publicly inherit from Event");

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

#define BIND_EVENT(oFunctionReference) std::bind(&oFunctionReference, this, std::placeholders::_1)

#define SUBMIT_EVENT(Event) events::EventBus::GetInstance().Submit(Event);
#define REGISTER_EVENT_LISTENER(EventListener) events::EventBus::GetInstance().Register(EventListener);
#define UNREGISTER_EVENT_LISTENER(EventListener) events::EventBus::GetInstance().UnRegister(EventListener);
