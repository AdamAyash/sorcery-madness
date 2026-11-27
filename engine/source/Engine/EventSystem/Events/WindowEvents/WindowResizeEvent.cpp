#include "WindowResizeEvent.h"

namespace engine::events
{
	WindowResizeEvent::WindowResizeEvent(IN const int nWidth, IN const int nHeight)
		: m_nWidth(nWidth)
		, m_nHeight(nHeight)
	{
	}

	WindowResizeEvent::~WindowResizeEvent()
	{
	}

	int WindowResizeEvent::GetWidth() const
	{
		return m_nWidth;
	}

	int WindowResizeEvent::GetHeight() const
	{
		return m_nHeight;
	}
}