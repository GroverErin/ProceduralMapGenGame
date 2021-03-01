#include "AnimationProcess.h"
#include "Components/AnimationComponent.h"

namespace Exelius
{
	void AnimationProcess::Update(float deltaTime)
	{
		m_frameTime += deltaTime;

		if (m_frameTime >= m_frameRate / 60.0f)
		{
			m_pAnimationComponent->NextFrame();
			m_frameTime = 0.0f;
		}
	}
}