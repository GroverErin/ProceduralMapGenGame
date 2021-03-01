#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class PlayerComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(PlayerComponent)

		PlayerComponent(Actor* pOwner)
			: IActorComponent(pOwner)
		{
			//
		}

		virtual ~PlayerComponent() = default;

		virtual bool Initialize([[maybe_unused]] tinyxml2::XMLElement* pData) final override { return true; }
	};

	class ProjectileComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(ProjectileComponent)

			ProjectileComponent(Actor* pOwner)
			: IActorComponent(pOwner)
		{
			//
		}

		virtual ~ProjectileComponent() = default;

		virtual bool Initialize([[maybe_unused]] tinyxml2::XMLElement* pData) final override { return true; }
	};

	class WallComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(WallComponent)

			WallComponent(Actor* pOwner)
			: IActorComponent(pOwner)
		{
			//
		}

		virtual ~WallComponent() = default;

		virtual bool Initialize([[maybe_unused]] tinyxml2::XMLElement* pData) final override { return true; }
	};
}