#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class TransformComponent;

	class BoxRenderComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(BoxRenderComponent)

		BoxRenderComponent(Actor* pOwner)
			: IActorComponent(pOwner)
			, m_width(0)
			, m_height(0)
			, m_red(0)
			, m_green(0)
			, m_blue(0)
			, m_alpha(255)
			, m_pTransform(nullptr)
		{
			//
		}

		virtual ~BoxRenderComponent() = default;

		virtual bool Initialize(tinyxml2::XMLElement* pData) final override;
		virtual bool PostInit() final override;
		virtual void Render(const std::unique_ptr<Exelius::IGraphics>& graphics) final override;

	private:
		int32_t m_width;
		int32_t m_height;
		uint8_t m_red;
		uint8_t m_green;
		uint8_t m_blue;
		uint8_t m_alpha;
		TransformComponent* m_pTransform;
	};
}