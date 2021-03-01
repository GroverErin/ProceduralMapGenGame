#pragma once
#include "Components/Physics/IPhysicsComponent.h"
#include "Game/Physics/Box2D/Box2DPhysics.h"

namespace Exelius
{
	class TransformComponent;
	class IPhysicsManager;

	class Box2DPhysicsComponent final
		: public IPhysicsComponent
	{
	public:

		Box2DPhysicsComponent(Actor* pOwner, IPhysicsManager* pPhysics);
		virtual ~Box2DPhysicsComponent();

		virtual bool Initialize(tinyxml2::XMLElement* pData) final override;

		virtual bool PostInit() final override;

		virtual void Update(float deltaTime) final override;

		virtual void Render(const std::unique_ptr<Exelius::IGraphics>& graphics) final override;

		virtual void SetPosition(Vector2f position) final override;
		virtual void SetDimensions(Vector2f dimensions) final override;
		virtual void ChangePosition(Vector2f position) final override;
		virtual void SetLinearVelocity(Vector2f velocity) final override;
		virtual void ApplyForce(const Vector2f& force, const Vector2f& point, bool wake) final override;
		virtual void ApplyForceToCenter(const Vector2f& force, bool wake) final override;
		virtual BodyType GetType() final override;

		bool IsColliding();
		bool IsCollidingWith(const std::string& category);

		void EnablePhysics(bool enable);

		uint16_t ParseMaskString(const std::string& mask);

		b2Fixture* GetFixture() const { return m_pFixture; }
		b2AABB GetBoundingBox() const;
		virtual const std::string& GetCategory() const final override { return m_category; }
		virtual Vector2f GetPositionCentered() const final override;

	private:

		b2World* m_pWorld;
		b2Body* m_pBody;
		b2Fixture* m_pFixture;

		// Shapes
		b2PolygonShape m_polyShape;

		// Definitions
		b2BodyDef m_bodyDef;
		b2FixtureDef m_fixtureDef;
		b2BodyType m_bodyType;

		TransformComponent* m_pTransform;
		Box2DPhysics* m_pPhysics;

		// Properties
		float m_width;
		float m_height;
		float m_density;
		float m_friction;
		std::string m_category;
	};
}