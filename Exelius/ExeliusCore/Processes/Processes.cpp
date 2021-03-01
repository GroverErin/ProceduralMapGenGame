#include "Processes.h"

namespace Exelius
{
	void ProcessManager::UpdateProcesses(float deltaTime)
	{
		size_t processIndex = 0;
		while (processIndex < m_processes.size())
		{
			auto pProcess = m_processes[processIndex];
			if (pProcess->GetState() == IProcess::State::UNINITIALIZED)
			{
				if (pProcess->Initialize())
				{
					pProcess->Resume();
				}
				else
				{
					m_processes.erase(m_processes.begin() + processIndex);
					continue;
				}
			}

			if (pProcess->GetState() == IProcess::State::RUNNING)
			{
				pProcess->Update(deltaTime);
			}

			if (pProcess->IsDead())
			{
				IProcess::State state = pProcess->GetState();
				if (state == IProcess::State::SUCCEEDED)
				{
					pProcess->OnSuccess();
					auto child = pProcess->RemoveChild();
					if (child)
						AttachProcess(child);
				}
				else if (state == IProcess::State::FAILED)
				{
					pProcess->OnFailure();
				}
				else if (state == IProcess::State::ABORTED)
				{
					pProcess->OnAbort();
				}

				if (m_processes.size() > processIndex)
					m_processes.erase(m_processes.begin() + processIndex);
				continue;

			}
			++processIndex;
		}
	}

	void ProcessManager::AbortAllProcesses()
	{
		for (auto pProcess : m_processes)
		{
			if (pProcess->IsAlive())
			{
				pProcess->Aborted();
				pProcess->OnAbort();
			}
		}
		m_processes.clear();
	}
}