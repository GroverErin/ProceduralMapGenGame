#pragma once
#include "Processes.h"

namespace Exelius
{
	class AnimationComponent;

	class AnimationProcess
		: public IProcess
	{
	public:
		AnimationProcess(AnimationComponent* pAnimationComponent, float frameRate)
			: m_pAnimationComponent(pAnimationComponent)
			, m_frameRate(frameRate)
			, m_frameTime(0.0f)
		{
			//
		}
		virtual ~AnimationProcess() = default;

		virtual void Update(float deltaTime) final override;

	private:
		float m_frameRate;
		float m_frameTime;
		AnimationComponent* m_pAnimationComponent;
	};
}