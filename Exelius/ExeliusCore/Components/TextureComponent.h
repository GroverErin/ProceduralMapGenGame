#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class TransformComponent;
	class ITexture;

	class TextureComponent
		: public IActorComponent
	{
	public:

		DEFINE_COMPONENT(TextureComponent)

		TextureComponent(Actor* pOwner)
			: IActorComponent(pOwner)
			, m_pTransform(nullptr)
			, m_width(0)
			, m_height(0)
			, m_renderFlag(true)
			, m_angle(0.0)
		{

		}

		virtual bool Initialize(tinyxml2::XMLElement* pData) final override;
		virtual bool PostInit() final override;
		virtual void Render(const std::unique_ptr<Exelius::IGraphics>& graphics) final override;
		// Dali: set render flag to activate/deactivate render this object
		void SetActive(bool flag) { m_renderFlag = flag; }

		void SetAngle(float angle) { m_angle = angle; }
	private:
		TransformComponent* m_pTransform;
		uint32_t m_width;
		uint32_t m_height;
		// Dali: set render flag to activate/deactivate render this object
		bool m_renderFlag;

		float m_angle;

		//Texture used by this texture component.
		std::shared_ptr<ITexture> m_pTexture;
	};
}