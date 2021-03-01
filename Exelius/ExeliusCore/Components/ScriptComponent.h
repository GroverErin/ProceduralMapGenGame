#pragma once
#include "Game/Actor.h"
#include "Managers/ScriptManager.h"

namespace Exelius
{
	class ScriptComponent
		: public IActorComponent
	{
	public:

		DEFINE_COMPONENT(ScriptComponent)

		ScriptComponent(Actor* pOwner)
			: IActorComponent(pOwner)
		{

		}

		virtual ~ScriptComponent() = default;

		virtual bool Initialize(tinyxml2::XMLElement * pData) final override;

		virtual void RegisterWithScript() final override;

	private:
		std::string m_scriptPath;
	};
}