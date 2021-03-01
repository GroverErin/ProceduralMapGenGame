#include "BoxRenderComponent.h"
#include "TransformComponent.h"
#include "ApplicationLayer.h"

namespace Exelius
{
	bool BoxRenderComponent::Initialize(tinyxml2::XMLElement* pData)
	{
		auto pElement = pData->FirstChildElement("Color");
		if (pElement != nullptr)
		{
			m_red = (uint8_t)pElement->UnsignedAttribute("r");
			m_green = (uint8_t)pElement->UnsignedAttribute("g");
			m_blue = (uint8_t)pElement->UnsignedAttribute("b");
			m_alpha = (uint8_t)pElement->UnsignedAttribute("a");
		}

		pElement = pData->FirstChildElement("Dimensions");
		if (pElement != nullptr)
		{
			m_width = pElement->IntAttribute("width");
			m_height = pElement->IntAttribute("height");
		}

		return true;
	}

	bool BoxRenderComponent::PostInit()
	{
		m_pTransform = GetOwner()->GetComponent<TransformComponent>();

		if (m_pTransform == nullptr)
		{
			auto& logger = IApplicationLayer::GetInstance()->GetLogger();
			logger.LogSevere("Unable to find Transform Component on Actor.");
			return false;
		}
		return true;
	}

	void BoxRenderComponent::Render(const std::unique_ptr<Exelius::IGraphics>& graphics)
	{
		graphics->DrawSolidRectangle(
			static_cast<int32_t>(m_pTransform->GetX()),
			static_cast<int32_t>(m_pTransform->GetY()),
			m_width, m_height,
			m_red, m_green, m_blue, m_alpha);
	}
}