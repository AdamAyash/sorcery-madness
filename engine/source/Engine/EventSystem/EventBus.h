#pragma once

#include <Common/Common.h>

namespace engine
{
	class Event;
	class IEventListener;

	class EventBus final : public common::NonCopyableObject
	{
	private:
		EventBus();

	public:
		~EventBus();

	public:
		static EventBus &GetInstance();

		void Register(IN IEventListener *pEventListener);
		void UnRegister(IN IEventListener *pEventListener);

		void Sumbit(IN const Event &oEvent);

	private:
		static unsigned long s_lEventListenerSubscriptionId;
		std::vector<IEventListener *> m_oEventListeners;
	};

#define SUBMIT_EVENT(Event) EventBus::GetInstance().Sumbit(Event);
#define REGISTER_EVENT_LISTENER() EventBus::GetInstance().Register(this);
#define UNREGISTER_EVENT_LISTENER() EventBus::GetInstance().UnRegister(this);
}