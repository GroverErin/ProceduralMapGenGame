#include "IPhysicsComponent.h"

namespace Exelius
{
	IPhysicsComponent::IPhysicsComponent(Actor* pOwner)
		: IActorComponent(pOwner)
	{
		//
	}

	void IPhysicsComponent::BeginOverlap(Actor* pOther)
	{
		// Handle Begin Overlap
		if (auto& handle = m_collisionHandlers[static_cast<unsigned long long>(HandlerType::kBeginOverlap)]; handle)
		{
			handle(pOther);
		}
	}

	void IPhysicsComponent::EndOverlap(Actor* pOther)
	{
		// Handle End Overlap
		if (auto& handle = m_collisionHandlers[static_cast<unsigned long long>(HandlerType::kEndOverlap)]; handle)
		{
			handle(pOther);
		}
	}

	void IPhysicsComponent::BeginCollision(Actor* pOther)
	{
		// Handle Begin Collision
		if (auto& handle = m_collisionHandlers[static_cast<unsigned long long>(HandlerType::kBeginCollision)]; handle)
		{
			handle(pOther);
		}
	}

	void IPhysicsComponent::EndCollision(Actor* pOther)
	{
		// Handle End Collision
		if (auto& handle = m_collisionHandlers[static_cast<unsigned long long>(HandlerType::kEndCollision)]; handle)
		{
			handle(pOther);
		}
	}

	void IPhysicsComponent::HandleOverlapping(Actor* pOther)
	{
		// Handle Currently In Overlap
		if (auto& handle = m_collisionHandlers[static_cast<unsigned long long>(HandlerType::kOverlapping)]; handle)
		{
			handle(pOther);
		}
	}
}