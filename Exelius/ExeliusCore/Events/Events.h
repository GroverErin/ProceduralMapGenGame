#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

namespace Exelius
{
	typedef uint32_t EventType;

	class IEvent
	{
	public:
		virtual ~IEvent() = default;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
	};

	class EventManager
	{
	public:
		size_t AddEventListener(EventType type, std::function<void(IEvent*)> listener);
		void RemoveEventListener(EventType type, size_t index);

		void QueueEvent(std::unique_ptr<IEvent> pEvent);
		void AbortEvent(EventType type);

		void ProcessEvents();

		void ResetEventManager();
	private:
		std::unordered_map<EventType, std::vector<std::function<void(IEvent*)>>> m_eventListeners;
		std::vector<std::unique_ptr<IEvent>> m_eventQueue;
	};
}