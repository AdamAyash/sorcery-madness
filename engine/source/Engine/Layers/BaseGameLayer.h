#pragma once

#include "Base/ILayer.h"
#include "../EventSystem/IEventListener.h"

namespace engine
{
	class BaseGameLayer : public ILayer, public events::IEventListener
	{
	protected:
		BaseGameLayer();
	public:
		virtual ~BaseGameLayer();

	public:
		virtual bool OnAttach() override;
		virtual bool OnDetach() override;
		virtual void OnEvent(const events::Event& oEvent) override;
		virtual void Update() override;
		virtual void Draw() override;

	private:
		virtual void LoadContent() PURE;

	private:
	};
}