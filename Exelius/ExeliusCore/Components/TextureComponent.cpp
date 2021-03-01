#include "TextureComponent.h"
#include "ApplicationLayer.h"
#include "TransformComponent.h"

namespace Exelius
{
	bool TextureComponent::Initialize(tinyxml2::XMLElement* pData)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		//Should return the value of "path" in xml.
		const char* pFilePath = nullptr;
		if (pData->FindAttribute("path"))
		{
			pFilePath = pData->FindAttribute("path")->Value();
		}

		auto pGraphics = IApplicationLayer::GetInstance()->GetGraphics();
		auto pGameLayer = IApplicationLayer::GetInstance()->GetGameLayer();
		auto pResource = pGameLayer->GetResourceFile().LoadResource(pFilePath);

		if (pFilePath)
			m_pTexture = pGraphics->LoadTexture(pResource);

		if (!m_pTexture)
		{
			logger.LogSevere("m_pTexture failed to load, is nullptr.");
			return false;
		}

		auto pElement = pData->FirstChildElement("Dimensions");
		if (pElement)
		{
			m_width = pElement->IntAttribute("width");
			m_height = pElement->IntAttribute("height");
		}
		else
		{
			return false;
		}

		return true;
	}

	bool TextureComponent::PostInit()
	{
		m_pTransform = GetOwner()->GetComponent<TransformComponent>();

		if (!m_pTransform)
		{
			auto& logger = IApplicationLayer::GetInstance()->GetLogger();
			logger.LogSevere("Unable to find Transform Component on Actor.");
			return false;
		}

		return true;
	}

	void TextureComponent::Render(const std::unique_ptr<Exelius::IGraphics>& graphics)
	{
		if (m_renderFlag)
		{
			graphics->DrawTextureEx(m_pTexture.get(),
				static_cast<int32_t>(m_pTransform->GetX()),
				static_cast<int32_t>(m_pTransform->GetY()),
				m_width, m_height,
				0, 0, 0, 0,
				m_angle);
		}
	}
}
