// Exelius Game Engine
// Created By: Erin Grover
// File Last Edited: 9/30/2020
// Last Edit: Initial creation.
// Notes: Much of this physics system is currently borrowed from Dali Yi's Halo Engine.

#pragma once
#include "Utilities/Vector2.h"

#include <memory>

/// <summary>
/// Exelius Engine namespace. Used for all Engine related code.
/// </summary>
namespace Exelius
{
	class IPhysicsComponent;
	class Actor;

	/// <summary>
	/// The interface for physics engines, eg. Box2D.
	/// The manager maintains the "relationship" between the contact listener and the colliders themselves.
	/// 
	/// \todo Find out a way to prevent clients from calling the Create and Initialize Methods.
	/// 
	/// \b Example:
	/// ~~~~~
	/// //Create engine defined Physics manager.
	/// //This may become a client-defined setting in the future.
	/// // Since Box2D is the only available Physics engine,
	/// // It will always use that.
	/// IPhysicsManager* pPhysicsManager = IPhysicsManager::Create({0, 9.81}); //Pass in vector for gravity.
	/// pPhysicsManager->Initialize();
	/// 
	/// //-----------------
	/// // Inside Game Loop
	/// //-----------------
	/// pPhysicsManager->Update(deltaTime); //Pass in calculated deltaTime. 
	/// ~~~~~
	/// </summary>
	class IPhysicsManager
	{
	public:
		/// <summary>
		/// Calls the destructor of derived Physics Manager classes.
		/// </summary>
		virtual ~IPhysicsManager() = default;

		/// <summary>
		/// Calls the Initialization method of the inhereting manager class.
		/// \b Example: Box2DPhysics::Initialize()
		/// </summary>
		/// <returns>(bool) True if the initialization was a success, false otherwise.</returns>
		virtual bool Initialize() = 0;

		/// <summary>
		/// Calls the Update method of the inhereting manager class.
		/// \b Example: Box2DPhysics::Update()
		/// </summary>
		/// <param name="deltaTime">(float) Current frame time.</param>
		virtual void Update(float deltaTime) = 0;

		/// <summary>
		/// Render any debugging features provided by the inhereting management class.
		/// </summary>
		virtual void RenderDebug() = 0;

		virtual void HandleContacts() = 0;
		virtual void HandleOverlaps() = 0;

		virtual void ClearContacts() = 0;

		/// <summary>
		/// Create the physics component. This allows for separate constructor implementations.
		/// Such as Box2D requiring a physics world for the components.
		/// </summary>
		/// <param name="pOwner">(Actor*) The actor that owns this component.</param>
		/// <returns>A unique pointer to the component that is created.</returns>
		virtual std::unique_ptr<IPhysicsComponent> CreatePhysicsComponent(Actor* pOwner) = 0;

		/// <summary>
		/// Creates the instance of the inhereting physics management system.
		/// \b Example: Box2DPhysics
		/// </summary>
		/// <param name="gravity">(Vector2f) A vector representing the gravity of the world simulation.</param>
		/// <returns>(std::unique_ptr<IPhysicsManager>) A pointer to the physics management system that is created.</returns>
		static std::unique_ptr<IPhysicsManager> Create(Vector2f gravity);
	protected:
	private:

	};
}