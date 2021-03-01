#include "IPhysicsManager.h"
#include "Box2D/Box2DPhysics.h"
#include <memory>

namespace Exelius
{
	std::unique_ptr<IPhysicsManager> IPhysicsManager::Create(Vector2f gravity)
	{
		return std::make_unique<Box2DPhysics>(gravity);
	}
}