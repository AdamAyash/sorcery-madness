#pragma once

#include "../../Event.h"

namespace engine::events
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent();
		~WindowCloseEvent();

	public:
		EVENT(WindowCloseEvent);
	};
}
