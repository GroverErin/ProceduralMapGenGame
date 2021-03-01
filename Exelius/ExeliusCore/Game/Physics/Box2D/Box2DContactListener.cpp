#include "Box2DContactListener.h"
#include "ApplicationLayer.h"
#include <algorithm>

namespace Exelius
{
	void Box2DContactListener::BeginContact(b2Contact* pContact)
	{
		IApplicationLayer::GetInstance()->GetLogger().LogInfo("Began Contact!");
		HandleContacts(pContact, true);
	}

	void Box2DContactListener::EndContact(b2Contact* pContact)
	{
		IApplicationLayer::GetInstance()->GetLogger().LogInfo("End Contact!");
		HandleContacts(pContact, false);
	}

	void Box2DContactListener::ClearContacts()
	{
		m_contacts.clear();
	}

	void Box2DContactListener::AddOverlap(Contact& overlap)
	{
		Overlap newOverlap = { overlap.pActorA, overlap.pActorB };
		m_overlaps.emplace_back(newOverlap);
	}

	void Box2DContactListener::RemoveOverlap(Contact& overlap)
	{
		auto it = std::remove_if(m_overlaps.begin(), m_overlaps.end(),
			[overlap](Overlap o)
			{
				return (overlap.pActorA == o.pActorA && overlap.pActorB == o.pActorB) || (overlap.pActorB == o.pActorA && overlap.pActorA == o.pActorB);
			});

		m_overlaps.erase(it, m_overlaps.end());
	}

	void Box2DContactListener::ClearOverlaps()
	{
		m_overlaps.clear();
	}

	void Box2DContactListener::RemoveInvalidOverlappingPair()
	{
		// Erase pair that becomes expired
		auto it = std::remove_if(m_overlaps.begin(), m_overlaps.end(), [](Overlap& o) {
			return !o.pActorA || !o.pActorA->IsValid() || !o.pActorB || !o.pActorB->IsValid();
		});

		m_overlaps.erase(it, m_overlaps.end());
	}

	void Box2DContactListener::HandleContacts(b2Contact* pContact, bool isBeginContact)
	{
		b2Fixture* pFixtureA = pContact->GetFixtureA();
		b2Fixture* pFixtureB = pContact->GetFixtureB();

		b2Body* pBodyA = pFixtureA->GetBody();
		b2Body* pBodyB = pFixtureB->GetBody();

		Actor* pActorA = static_cast<Actor*>(pBodyA->GetUserData());
		Actor* pActorB = static_cast<Actor*>(pBodyB->GetUserData());

		// Make sure both actors are valid.
		if (!pActorA || !pActorB)
		{
			IApplicationLayer::GetInstance()->GetLogger().LogInfo("An actor was invalid!");
			return;
		}

		// \todo Log the actors that have collided.

		bool isOverlap = false;
		if (pFixtureA->IsSensor() || pFixtureB->IsSensor())
		{
			isOverlap = true;
		}

		Contact contact = { pActorA, pActorB, isOverlap, isBeginContact };
		m_contacts.emplace_back(contact);
	}
}