// Exelius Game Engine
// Created By: Erin Grover
// File Last Edited: 9/19/2020
// Last Edit: Initial creation.

#pragma once
#include <ThirdParty/Middleware/Box2D/include/box2d.h>
#include "Game/Physics/IPhysicsManager.h"
#include "Box2DContactListener.h"
#include "Utilities/Shapes/Shapes.h"
#include <string>
#include <unordered_map>

/// <summary>
/// Exelius Engine namespace. Used for all Engine related code.
/// </summary>
namespace Exelius
{
	class DebugDrawPhysics
		: public b2Draw
	{
		std::vector<FPoint> m_points;
	public:
		// Inherited via b2Draw
		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
		virtual void DrawCircle([[maybe_unused]] const b2Vec2& center, [[maybe_unused]] float radius, [[maybe_unused]] const b2Color& color) override {}
		virtual void DrawSegment([[maybe_unused]] const b2Vec2& p1, [[maybe_unused]] const b2Vec2& p2, [[maybe_unused]] const b2Color& color) override {}
		virtual void DrawTransform([[maybe_unused]] const b2Transform& xf) override {}
		virtual void DrawPoint([[maybe_unused]] const b2Vec2& p, [[maybe_unused]] float size, [[maybe_unused]] const b2Color& color) override {}
		virtual void DrawSolidPolygon([[maybe_unused]] const b2Vec2* vertices, [[maybe_unused]] int32 vertexCount, [[maybe_unused]] const b2Color& color) override {}
		virtual void DrawSolidCircle([[maybe_unused]] const b2Vec2& center, [[maybe_unused]] float radius, [[maybe_unused]] const b2Vec2& axis, [[maybe_unused]] const b2Color& color) override {}
	};

	/// <summary>
	/// The interface for the Box2D Physics engine.
	/// </summary>
	class Box2DPhysics final
		: public IPhysicsManager
	{
		/// <summary>
		/// \todo Make the Box2D constants configurable.
		/// </summary>
		static constexpr int kVelocityIterations = 8;
		static constexpr int kPositionIterations = 3;

	public:
		static constexpr float kPixelToMeterConversion = 1.0f / 50.0f;

		/// <summary>
		/// Called using the Static Create function.
		/// \b Note: Does not initialize the physics manager.
		/// </summary>
		/// <param name="gravityVector"></param>
		Box2DPhysics(Vector2f gravityVector);

		//--------------------------------------------------------------------------------------------------
		// Inhereting from IPhysicsManager
		//--------------------------------------------------------------------------------------------------

		virtual bool Initialize() final override;
		virtual void Update(float deltaTime) final override;
		virtual void RenderDebug() final override;
		virtual std::unique_ptr<IPhysicsComponent> CreatePhysicsComponent(Actor* pOwner) final override;

		virtual void HandleContacts() final override;
		virtual void HandleOverlaps() final override;

		virtual void ClearContacts() final override;

		uint16_t GetColliderCode(const std::string& category);
		b2World* GetWorld() { return &m_world; }

		Box2DContactListener& GetContactListener() { return m_contactListener; }
	private:


		static constexpr float kPhysicsTimeStep = 1.0f / 60.0f;
		float m_currentPhysicsStepTime;
		static constexpr float kOverlapStepTime = 0.2f;
		float m_currentOverlapSetTime;

		b2World m_world;
		Box2DContactListener m_contactListener;
		DebugDrawPhysics m_debugDraw;

		// Collider Category
		std::unordered_map<std::string, uint16_t> m_colliderCategories;
	};
}