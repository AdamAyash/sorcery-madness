#pragma once

#include "../../Event.h"

namespace engine::events
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(IN const int nWidth, IN const int nHeight);
		~WindowResizeEvent();

	public:
		int GetWidth() const;
		int GetHeight() const;

	public:
		EVENT(WindowResizeEvent)

	private:
		const int m_nWidth;
		const int m_nHeight;
	};
}