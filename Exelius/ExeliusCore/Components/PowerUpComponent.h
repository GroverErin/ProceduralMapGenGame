#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class PowerUpComponent : public IActorComponent
	{
	public:
		DEFINE_COMPONENT(PowerUpComponent)

		PowerUpComponent(Actor* pOwner)
			: IActorComponent(pOwner)
		{
			//
		}

		virtual ~PowerUpComponent() = default;

		virtual bool Initialize([[maybe_unused]] tinyxml2::XMLElement* pData) final override { return true; }
	};
}