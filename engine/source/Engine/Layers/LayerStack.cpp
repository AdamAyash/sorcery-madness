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

	bool LayerStack::PushLayer(IN ILayer* const oLayer)
	{
		if (!oLayer->OnAttach())
		{
			LOG_ERROR("Failed to attach layer");
			return false;
		}

		m_oLayers.push_back(oLayer);

		return true;
	}

	void LayerStack::OnEvent(events::Event& oEvent)
	{
		if (oEvent.bIsHandled)
			return;

		for (size_t nIndex = m_oLayers.size(); nIndex > 0; nIndex--)
		{
			const auto pCurrentLayer = m_oLayers.at(nIndex - 1);
			if (!pCurrentLayer)
				continue;

			pCurrentLayer->OnEvent(oEvent);
		}
	}

	void LayerStack::OnUpdate()
	{
		for (size_t nIndex = m_oLayers.size(); nIndex > 0; nIndex--)
		{
			const auto pCurrentLayer = m_oLayers.at(nIndex - 1);
			if (!pCurrentLayer)
				continue;

			pCurrentLayer->OnUpdate();
		}
	}

	void LayerStack::Draw(IN Renderer2D& oRenderer2D)
	{
		for (size_t nIndex = m_oLayers.size(); nIndex > 0; nIndex--)
		{
			const auto pCurrentLayer = m_oLayers.at(nIndex - 1);
			if (!pCurrentLayer)
				continue;

			pCurrentLayer->Draw(oRenderer2D);
		}
	}

	void LayerStack::Clear()
	{
		for (size_t nIndex = m_oLayers.size(); nIndex > 0; nIndex--)
		{
			const auto pCurrentLayer = m_oLayers.at(nIndex - 1);
			if (!pCurrentLayer)
				continue;

			pCurrentLayer->OnDetach();

			delete pCurrentLayer;
		}

		m_oLayers.clear();
	}
}