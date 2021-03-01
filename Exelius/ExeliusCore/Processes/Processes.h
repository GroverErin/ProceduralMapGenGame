#pragma once
#include <memory>
#include <vector>
#include <functional>

namespace Exelius
{
	class IProcess
	{
	public:
		enum class State
		{
			UNINITIALIZED,
			RUNNING,
			PAUSED,
			SUCCEEDED,
			FAILED,
			ABORTED
		};

		IProcess()
			: m_state(State::UNINITIALIZED)
		{
			//
		}

		virtual ~IProcess() = default;

		//States

		virtual bool Initialize() { return true; }
		virtual void Update(float deltaTime) = 0;

		void AttachChild(std::shared_ptr<IProcess> pProcess)
		{
			m_pChild = pProcess;
		}

		std::shared_ptr<IProcess> RemoveChild()
		{
			auto pChild = m_pChild;
			m_pChild = nullptr;
			return pChild;
		}

		void SetSuccessCallback(std::function<void()> callback)
		{
			m_successCallback = callback;
		}

		void OnSuccess()
		{
			if (m_successCallback)
				m_successCallback();
		}

		void SetFailureCallback(std::function<void()> callback)
		{
			m_failureCallback = callback;
		}

		void OnFailure()
		{
			if (m_failureCallback)
				m_failureCallback();
		}

		void SetAbortCallback(std::function<void()> callback)
		{
			m_abortCallback = callback;
		}

		void OnAbort()
		{
			if (m_abortCallback)
				m_abortCallback();
		}

		void Succeeded() { m_state = State::SUCCEEDED; }
		void Failed() { m_state = State::FAILED; }
		void Aborted() { m_state = State::ABORTED; }
		void Pause() { m_state = State::PAUSED; }
		void Resume() { m_state = State::RUNNING; }

		State GetState() const { return m_state; }

		bool IsAlive() const
		{
			return m_state == State::RUNNING || m_state == State::PAUSED;
		}

		bool IsDead() const
		{
			return m_state == State::SUCCEEDED
				|| m_state == State::FAILED
				|| m_state == State::ABORTED;
		}

	private:
		State m_state;
		std::shared_ptr<IProcess> m_pChild;
		std::function<void()> m_successCallback;
		std::function<void()> m_failureCallback;
		std::function<void()> m_abortCallback;
	};

	class ProcessManager
	{
	public:
		~ProcessManager() { AbortAllProcesses(); }

		void UpdateProcesses(float deltaTime);

		void AttachProcess(std::shared_ptr<IProcess> pProcess)
		{
			m_processes.emplace_back(pProcess);
		}

		void AbortAllProcesses();

		std::vector<std::shared_ptr<IProcess>>& GetAllProcesses()
		{
			return m_processes;
		}

	private:
		std::vector<std::shared_ptr<IProcess>> m_processes;
	};
}