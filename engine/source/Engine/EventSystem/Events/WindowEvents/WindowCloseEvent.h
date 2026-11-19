#pragma once

#include "../../Event.h"

namespace engine
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
