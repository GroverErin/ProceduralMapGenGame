#pragma once
#include "Events.h"
#include "Game/Actor.h"

namespace Exelius
{
	class DestroyActorEvent
		: public IEvent
	{
	public:
		static const EventType kEventType = 0xdc7a514e;

		DestroyActorEvent(Actor::Id id)
			: m_id(id)
		{
			//
		}

		virtual ~DestroyActorEvent() = default;

		virtual EventType GetEventType() const final override
		{
			return kEventType;
		}

		virtual const char* GetName() const final override
		{
			return "DestroyActor";
		}

		Actor::Id GetId() const
		{
			return m_id;
		}

	private:
		Actor::Id m_id;
	};
}