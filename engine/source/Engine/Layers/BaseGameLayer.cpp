#include "BaseGameLayer.h"
#include "../EventSystem/EventDispatcher.h"

namespace engine
{
	BaseGameLayer::BaseGameLayer()
	{
	}

	BaseGameLayer::~BaseGameLayer()
	{
	}

	bool BaseGameLayer::OnAttach()
	{
		if (!this->LoadContent())
		{
			//TODO LOG
			return false;
		}

		return true;
	}

	bool BaseGameLayer::OnDetach()
	{
		return true;
	}

	void BaseGameLayer::OnUpdate()
	{
		for (auto& gameObjectIterator : m_oGameObjects)
		{
			gameObjectIterator->Update();
		}
	}

	void BaseGameLayer::Draw(IN Renderer2D& oRenderer2D)
	{
		for (auto& gameObjectIterator : m_oGameObjects)
		{
			gameObjectIterator->Draw(oRenderer2D);
		}
	}
}