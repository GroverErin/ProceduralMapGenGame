#include "Box2DPhysics.h"
#include "ApplicationLayer.h"
#include "Components/Physics/Box2D/Box2DPhysicsComponent.h"
#include "Utilities/Color.h"

namespace Exelius
{
	Box2DPhysics::Box2DPhysics(Vector2f gravityVector)
		: m_currentPhysicsStepTime(0.0f)
		, m_currentOverlapSetTime(0.0f)
		, m_world(b2Vec2(gravityVector.x, gravityVector.y))
		, m_contactListener()
	{
		//
	}

	bool Box2DPhysics::Initialize()
	{
		IApplicationLayer::GetInstance()->GetLogger().LogInfo("Initializing Box2D Physics!");
		m_world.SetContactListener(&m_contactListener);

		//May need DebugDraw init methods here.
		m_world.SetDebugDraw(&m_debugDraw);

		m_debugDraw.SetFlags(b2Draw::e_aabbBit);
	
		return true;
	}

	void Box2DPhysics::Update(float deltaTime)
	{
		//Update timers
		m_currentPhysicsStepTime += deltaTime;

		//If physics timer has hit limit, step the physics engine, and reset the timer.
		if (m_currentPhysicsStepTime >= kPhysicsTimeStep)
		{
			m_currentPhysicsStepTime -= kPhysicsTimeStep;
			m_world.Step(kPhysicsTimeStep, kVelocityIterations, kPositionIterations);
		}

		m_currentOverlapSetTime += deltaTime;
	}

	void Box2DPhysics::RenderDebug()
	{
		m_world.DebugDraw();
	}

	std::unique_ptr<IPhysicsComponent> Box2DPhysics::CreatePhysicsComponent(Actor* pOwner)
	{
		return std::make_unique<Box2DPhysicsComponent>(pOwner, this);
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------
	// Private Functions
	//--------------------------------------------------------------------------------------------------------------------------------------------

	void Box2DPhysics::HandleContacts()
	{
		//Handle all collision updates.
		auto& beginContacts = m_contactListener.GetBeginContacts();
		for (auto& contact : beginContacts)
		{
			//If either of the actors involved is a box 2d sensor...
			if (contact.isOverlap)
			{
				if (contact.isBeginContact)
				{
					contact.pActorA->GetComponent<Box2DPhysicsComponent>()->BeginOverlap(contact.pActorB);
					contact.pActorB->GetComponent<Box2DPhysicsComponent>()->BeginOverlap(contact.pActorA);
					//Has begun overlapping.
					m_contactListener.AddOverlap(contact);
				}
				else
				{
					contact.pActorA->GetComponent<Box2DPhysicsComponent>()->EndOverlap(contact.pActorB);
					contact.pActorB->GetComponent<Box2DPhysicsComponent>()->EndOverlap(contact.pActorA);
					//Has stopped overlapping.
					m_contactListener.RemoveOverlap(contact);
				}
			}
			else
			{
				if (contact.isBeginContact)
				{
					contact.pActorA->GetComponent<Box2DPhysicsComponent>()->BeginCollision(contact.pActorB);
					contact.pActorB->GetComponent<Box2DPhysicsComponent>()->BeginCollision(contact.pActorA);
				}
				else
				{
					contact.pActorA->GetComponent<Box2DPhysicsComponent>()->EndCollision(contact.pActorB);
					contact.pActorB->GetComponent<Box2DPhysicsComponent>()->EndCollision(contact.pActorA);
				}
			}
		}

		//Collisions have been handled, so clear the list.
		m_contactListener.ClearContacts();
	}

	void Box2DPhysics::HandleOverlaps()
	{
		//If overlap timer has hit the limit, update all overlapping contacts, and reset the timer.
		if (m_currentOverlapSetTime >= kOverlapStepTime)
		{
			auto& overlapContacts = m_contactListener.GetOverlappingContacts();
			for (auto& contact : overlapContacts)
			{
				// Handle Begin Overlap
				contact.pActorA->GetComponent<Box2DPhysicsComponent>()->HandleOverlapping(contact.pActorB);
				contact.pActorB->GetComponent<Box2DPhysicsComponent>()->HandleOverlapping(contact.pActorA);
			}
			m_currentOverlapSetTime = 0.0f;
		}
	}

	uint16_t Box2DPhysics::GetColliderCode(const std::string& category)
	{
		size_t size = m_colliderCategories.size();

		if (size >= 16)
		{
			//LOG_FATAL("Too Many Box2D Physics Category!");
			return 0;
		}

		auto it = m_colliderCategories.find(category);
		if (it == m_colliderCategories.end())
		{
			uint16_t val = 1 << static_cast<uint16_t>(size);
			it = m_colliderCategories.emplace(category, val).first;
			//LOG_DISPLAY("New Physics Category Added!");
		}

		return it->second;
	}

	void Box2DPhysics::ClearContacts()
	{
		m_contactListener.ClearContacts();
		m_contactListener.ClearOverlaps();
	}

	void DebugDrawPhysics::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		Color clr((uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255, 255));
		// fill points
		while (vertexCount--)
		{
			m_points.emplace_back(FPoint(vertices->x, vertices->y));
			++vertices;
		}
		// end points
		m_points.emplace_back(m_points[0]);
		// Draw
		IApplicationLayer::GetInstance()->GetGraphics()->DrawPolygon(m_points, clr);
		m_points.clear();
	}
}