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
		events::REGISTER_EVENT_LISTENER(this);

		this->LoadContent();
		return true;
	}

	bool BaseGameLayer::OnDetach()
	{
		return true;
	}

	void BaseGameLayer::OnEvent(const events::Event& oEvent)
	{

	}

	void BaseGameLayer::Update()
	{
	
	}

	void BaseGameLayer::Draw()
	{
		
	}
}