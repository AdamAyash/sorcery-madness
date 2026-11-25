#pragma once

#include "Base/ILayer.h"
#include "../EventSystem/IEventListener.h"
#include "../GameObjects/GameObject.h"

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
		virtual void Draw(IN Renderer2D& oRenderer2D) override;

	protected:
		template<class TGameObject>
		void AddGameObject(IN TGameObject& oGameObject)
		{
			static_assert(std::is_convertible_v<TGameObject*, GameObject*>,
				"TGameObject must publicly inherit from GameObject");

			m_oGameObjects.push_back(&oGameObject);
		}

	private:
		virtual bool LoadContent() PURE;

	private:
		GameObjectsList m_oGameObjects;
	};
}