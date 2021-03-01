#pragma once
#include "Events.h"

namespace Exelius
{
	class CreateActorEvent
		: public IEvent
	{
	public:
		static const EventType kEventType = 0xc9a8569;

		CreateActorEvent(const char* pXMLFile, bool enemy)
			: m_pXMLFile(pXMLFile)
			, m_enemy(enemy)
		{
			//
		}
		virtual ~CreateActorEvent() = default;

		virtual EventType GetEventType() const final override
		{
			return kEventType;
		}

		virtual const char* GetName() const final override
		{
			return "CreateActor";
		}

		const char* GetXMLFile() const
		{
			return m_pXMLFile;
		}

		bool IsEnemy() const
		{
			return m_enemy;
		}

	private:
		const char* m_pXMLFile;
		bool m_enemy;
	};
}