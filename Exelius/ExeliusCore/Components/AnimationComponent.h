#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class TransformComponent;
	class IProcess;
	class ITexture;

	class AnimationComponent
		: public IActorComponent
	{
	public:

		DEFINE_COMPONENT(AnimationComponent)

		AnimationComponent(Actor* pOwner)
			: IActorComponent(pOwner)
			, m_pTransform(nullptr)
			, m_currentFrameIndex(0)
			, m_isReversed(false)
			, m_paused(false)
			, m_pCurrentAnimation(nullptr)
			, m_renderFlag(true)
		{
		}

		virtual bool Initialize(tinyxml2::XMLElement* pData) final override;
		virtual bool PostInit() final override;
		virtual void Render(const std::unique_ptr<Exelius::IGraphics>& graphics) final override;

		void Pause(bool pause);
		bool Resume(const std::string& animationName);
		void Reverse();
		void NextFrame();
		// Dali: set render flag to activate/deactivate render this object
		void SetActive(bool flag) { m_renderFlag = flag; }

	private:
		void CreateAnimationProcess();

		struct Frame
		{
			uint16_t fxPos;
			uint16_t fyPos;
			uint16_t fwidth;
			uint16_t fheight;
		};

		using Sequence = std::vector<int>;
		using Frames = std::vector<Frame>;

		std::shared_ptr<IProcess> m_pAnimationProcess;

		//Transform that this animation will render to.
		TransformComponent* m_pTransform;

		//Texture used by this animation component.
		std::shared_ptr<ITexture> m_pTexture;

		//List of frames that this animation component has. Prevents potential memory waste.
		Frames m_frames;

		//Container of all animations in this component.
		std::unordered_map<std::string, Sequence> m_animations;
		
		//Current animation stats.
		bool m_isReversed;
		bool m_paused;
		// Dali: set render flag to activate/deactivate render this object
		bool m_renderFlag;

		uint16_t m_currentFrameIndex;

		Sequence* m_pCurrentAnimation;
	};
}