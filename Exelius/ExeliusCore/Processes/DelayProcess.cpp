#include "DelayProcess.h"

namespace Exelius
{
	void DelayProcess::Update(float deltaTime)
	{
		m_delay -= deltaTime;
		if (m_delay <= 0.0f)
		{
			Succeeded();
		}
	}
}