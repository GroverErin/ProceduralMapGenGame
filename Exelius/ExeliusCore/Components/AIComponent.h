#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class AIComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(AIComponent)

			AIComponent(Actor* pOwner)
			: IActorComponent(pOwner)
		{
			//
		}

		virtual ~AIComponent() = default;

		virtual bool Initialize([[maybe_unused]] tinyxml2::XMLElement* pData) final override { return true; }
	};
}