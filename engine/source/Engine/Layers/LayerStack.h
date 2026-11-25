#pragma once

#include <Common/Common.h>
#include <vector>
#include "../Graphics/Rendrer2D/Renderer2D.h"

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

		void Update();
		void Draw(IN Renderer2D& oRenderer2D);

		void Clear();

	private:
		std::vector<ILayer*> m_oLayers;
	};
}