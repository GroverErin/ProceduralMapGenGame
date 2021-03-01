#pragma once
#include "Processes.h"

namespace Exelius
{
	class DelayProcess
		: public IProcess
	{
	public:
		DelayProcess(float delay)
			: m_delay(delay)
		{
			//
		}
		virtual ~DelayProcess() = default;

		virtual void Update(float deltaTime) final override;

	private:
		float m_delay;
	};
}