#include "Events.h"

size_t Exelius::EventManager::AddEventListener(EventType type, std::function<void(IEvent*)> listener)
{
	//Check if there ar any holes in the map, and fill them if so.
	auto& listeners = m_eventListeners[type];
	for (size_t index = 0; index < listeners.size(); ++index)
	{
		if (!listeners[index])
		{
			listeners[index] = listener;
			return index;
		}
	}
	
	//No empty places were found, add it to the end.
	listeners.emplace_back(std::move(listener));
	return listeners.size() - 1;
}

void Exelius::EventManager::RemoveEventListener(EventType type, size_t index)
{
	auto& listeners = m_eventListeners[type];
	if (index < listeners.size())
	{
		listeners[index] = nullptr;
	}
}

void Exelius::EventManager::QueueEvent(std::unique_ptr<IEvent> pEvent)
{
	m_eventQueue.emplace_back(std::move(pEvent));
}

void Exelius::EventManager::AbortEvent(EventType type)
{
	auto itr = m_eventQueue.begin();

	while (itr != m_eventQueue.end())
	{
		if ((*itr)->GetEventType() == type)
		{
			itr = m_eventQueue.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void Exelius::EventManager::ProcessEvents()
{
	//Move this, so any new events created from processing this queue will be proccessed next frame.
	auto processingQueue = std::move(m_eventQueue);
	for (auto& pEvent : processingQueue)
	{
		auto& listeners = m_eventListeners.at(pEvent->GetEventType());
		for (auto& listener : listeners)
		{
			if (listener)
			{
				listener(pEvent.get());
			}
		}
	}
}

void Exelius::EventManager::ResetEventManager()
{
	m_eventListeners.clear();
	m_eventQueue.clear();
}
