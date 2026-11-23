#pragma once

#include <Common/Common.h>

namespace engine::events
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
		std::unordered_map<unsigned long, IEventListener *> m_oEventListenersMap;
	};
}