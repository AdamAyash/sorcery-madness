#pragma once

#include <Common/Common.h>
#include <vector>
#include "../Graphics/Renderer2D/Renderer2D.h"
#include "../EventSystem/Event.h"

namespace engine
{
	class ILayer;

	class LayerStack final : public common::NonCopyableObject
	{
	public:
		LayerStack();
		~LayerStack();

	public:
		bool PushLayer(IN ILayer* const oLayer);

		void OnEvent(events::Event& oEvent);
		void OnUpdate();
		void Draw(IN Renderer2D& oRenderer2D);

		void Clear();

	private:
		std::vector<ILayer*> m_oLayers;
	};
}