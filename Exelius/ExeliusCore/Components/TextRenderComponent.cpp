#include "TextRenderComponent.h"
#include "ApplicationLayer.h"
#include "TransformComponent.h"

namespace Exelius
{
	bool TextRenderComponent::Initialize(tinyxml2::XMLElement* pData)
	{
		auto pApplicationLayer = IApplicationLayer::GetInstance();
		auto& logger = pApplicationLayer->GetLogger();

		const char* pFontPath = nullptr;
		if (pData->FindAttribute("path"))
		{
			pFontPath = pData->FindAttribute("path")->Value();
		}
		else
		{
			logger.LogSevere("Failed to find Font Path.");
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
			logger.LogSevere("Failed to find Dimensions.");
			return false;
		}

		uint8_t red = 255;
		uint8_t green = 255;
		uint8_t blue = 255;
		uint8_t alpha = 255;
		pElement = pData->FirstChildElement("Color");
		if (pElement)
		{
			red = static_cast<uint8_t>(pElement->IntAttribute("red"));
			green = static_cast<uint8_t>(pElement->IntAttribute("green"));
			blue = static_cast<uint8_t>(pElement->IntAttribute("blue"));
			alpha = static_cast<uint8_t>(pElement->IntAttribute("alpha"));
		}
		else
		{
			logger.LogSevere("Failed to find Dimensions.");
			return false;
		}

		pElement = pData->FirstChildElement("Size");
		int size = 0;
		if (pElement)
		{
			size = pElement->IntText();
		}
		else
		{
			logger.LogSevere("Failed to find Font Size.");
			return false;
		}

		pElement = pData->FirstChildElement("Text");
		const char* pText = nullptr;
		if (pElement)
		{
			pText = pElement->GetText();
		}
		else
		{
			logger.LogSevere("Failed to find Text for TextRenderComponent");
			return false;
		}

		auto pGraphics = pApplicationLayer->GetGraphics();
		auto pGameLayer = pApplicationLayer->GetGameLayer();
		m_pResource = pGameLayer->GetResourceFile().LoadResource(pFontPath);

		if (m_pResource)
		{
			m_pFont = pGraphics->LoadFont(m_pResource, size);
		}
		else
		{
			logger.LogSevere("Font resource failed to load.");
			return false;
		}

		if (!m_pFont)
		{
			logger.LogSevere("Font load has failed.");
			return false;
		}

		m_pRenderText = pGraphics->LoadText(pText, m_pFont, red, green, blue, alpha);

		if (!m_pRenderText)
		{
			logger.LogSevere("Text load has failed.");
			return false;
		}

		return true;
	}

	bool TextRenderComponent::PostInit()
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

	void TextRenderComponent::Render(const std::unique_ptr<Exelius::IGraphics>& graphics)
	{
		if (m_pRenderText && m_renderFlag)
		{
			graphics->DrawText(m_pRenderText.get()
				, static_cast<int32_t>(m_pTransform->GetX())
				, static_cast<int32_t>(m_pTransform->GetY())
				, m_width, m_height);
		}
	}

	void TextRenderComponent::ChangeText(const char* pNewText, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		auto pGraphics = IApplicationLayer::GetInstance()->GetGraphics();

		m_pRenderText = pGraphics->LoadText(pNewText, m_pFont, red, green, blue, alpha);
		
		int width, height;
		m_pRenderText->GetTextureDimensions(width, height);
		m_width = width;
		m_height = height;
	}
}