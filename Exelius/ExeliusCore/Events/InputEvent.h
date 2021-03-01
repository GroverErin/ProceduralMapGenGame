#pragma once
#include "Events.h"

namespace Exelius
{
	class InputEvent
		: public IEvent
	{
	public:
		static const EventType kEventType = 0x4693700;

		InputEvent(bool isDown, size_t key)
			: m_isDown(isDown)
			, m_key(key)
		{
			//
		}

		virtual ~InputEvent() = default;

		virtual EventType GetEventType() const final override
		{
			return kEventType;
		}

		virtual const char* GetName() const final override
		{
			return "Input";
		}

	private:
		bool m_isDown;
		size_t m_key;
	};
}