#pragma once

#include <Common/Common.h>
#include <vector>

namespace engine
{
	class ILayer;

	class LayerStack final : public common::NonCopyableObject
	{
	public:
		LayerStack();
		~LayerStack();

	public:
		void PushLayer(IN ILayer* const oLayer);

		void Update();
		void Draw();

		void Clear();

	private:
		std::vector<ILayer*> m_oLayers;
	};
}