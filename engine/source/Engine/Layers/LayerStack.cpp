#include "LayerStack.h"
#include "Base/ILayer.h"

namespace engine
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::PushLayer(IN ILayer* const oLayer)
	{
		m_oLayers.push_back(oLayer);
		oLayer->OnAttach();
	}

	void LayerStack::Update()
	{
		for (size_t nIndex = 0; nIndex < m_oLayers.size(); nIndex++)
		{
			const auto pCurrentLayer = m_oLayers.at(nIndex);
			if (!pCurrentLayer)
				continue;

			pCurrentLayer->Update();
		}
	}

	void LayerStack::Draw()
	{
		for (size_t nIndex = 0; nIndex < m_oLayers.size(); nIndex++)
		{
			const auto pCurrentLayer = m_oLayers.at(nIndex);
			if (!pCurrentLayer)
				continue;

			pCurrentLayer->Draw();
		}
	}

	void LayerStack::Clear()
	{
		for (size_t nIndex = 0; nIndex < m_oLayers.size(); nIndex++)
		{
			const auto pCurrentLayer = m_oLayers.at(nIndex);
			if (!pCurrentLayer)
				continue;

			pCurrentLayer->OnDetach();

			delete pCurrentLayer;
		}

		m_oLayers.clear();
	}
}