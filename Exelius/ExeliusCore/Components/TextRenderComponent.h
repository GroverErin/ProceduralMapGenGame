#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class IFont;
	class ITexture;
	class TransformComponent;

	class TextRenderComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(TextRenderComponent)

		TextRenderComponent(Actor* pOwner)
			: IActorComponent(pOwner)
			, m_pFont(nullptr)
			, m_pRenderText(nullptr)
			, m_pTransform(nullptr)
			, m_width(0)
			, m_height(0)
			, m_renderFlag(true)
		{
			//
		}

		virtual bool Initialize(tinyxml2::XMLElement* pData) final override;

		virtual bool PostInit() final override;

		virtual void Render(const std::unique_ptr<Exelius::IGraphics>& graphics) final override;

		// Dali: set render flag to activate/deactivate render this object
		void SetActive(bool flag) { m_renderFlag = flag; }

		void ChangeText(const char* pNewText, uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255);

	private:
		std::shared_ptr<IFont> m_pFont;
		std::shared_ptr<ITexture> m_pRenderText;
		std::shared_ptr<Resource> m_pResource;
		TransformComponent* m_pTransform;


		int m_width;
		int m_height;
		// Dali: set render flag to activate/deactivate render this object
		bool m_renderFlag;
	};
}