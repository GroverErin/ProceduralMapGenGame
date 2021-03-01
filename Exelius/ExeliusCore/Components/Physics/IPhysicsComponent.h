#pragma once
#include "Game/Actor.h"
#include "Utilities/Vector2.h"

#include <functional>
#include <string>

namespace Exelius
{
	class IPhysicsComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(IPhysicsComponent);

		using CollisionHandler = std::function<void(Actor* pOther)>;

		// Handler Enum
		enum class HandlerType : uint8_t
		{
			kBeginOverlap = 0,
			kEndOverlap,
			kBeginCollision,
			kEndCollision,
			kOverlapping,
			kTotal,
		};

		enum class BodyType : uint8_t
		{
			kStatic,
			kDynamic,
			kKinetic
		};

		IPhysicsComponent(Actor* pOwner);

		virtual ~IPhysicsComponent() = default;

		virtual void SetPosition(Vector2f position) = 0;
		virtual void SetDimensions(Vector2f dimensions) = 0;
		virtual void ChangePosition(Vector2f position) = 0;
		virtual void SetLinearVelocity(Vector2f velocity) = 0;
		virtual void ApplyForce(const Vector2f& force, const Vector2f& point, bool wake) = 0;
		virtual void ApplyForceToCenter(const Vector2f& force, bool wake) = 0;
		virtual BodyType GetType() = 0;
		virtual const std::string& GetCategory() const = 0;
		virtual Vector2f GetPositionCentered() const = 0;

		void BeginOverlap(Actor* pOther);
		void EndOverlap(Actor* pOther);
		void BeginCollision(Actor* pOther);
		void EndCollision(Actor* pOther);
		void HandleOverlapping(Actor* pOther);

		void SetCollisionHandler(HandlerType type, CollisionHandler handler) { m_collisionHandlers[static_cast<unsigned long long>(type)] = handler; }

	private:
		CollisionHandler m_collisionHandlers[static_cast<unsigned long long>(HandlerType::kTotal)];
	};
}