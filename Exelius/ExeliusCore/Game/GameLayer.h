#pragma once
#include "Utilities/Vector2.h"

#include "View.h"
#include "Actor.h"
#include "Processes/Processes.h"
#include "Events/Events.h"
#include "Managers/ScriptManager.h"

#include <vector>
#include <unordered_map>
#include <string>

template <class Type>
static std::unique_ptr<Type> CreateComponent(Exelius::Actor* pOwner)
{
	return std::make_unique<Type>(pOwner);
}

#define REGISTER_COMPONENT_CREATOR(_component) \
	m_actorFactory.RegisterComponentCreator(#_component, &CreateComponent<Exelius::_component>)

namespace Exelius
{
	class IPhysicsManager;

	class IGameLayer
	{
	public:
		IGameLayer();

		virtual ~IGameLayer();

		virtual const char* GetGameName() const = 0;

		virtual bool Initialize();

		virtual void AddView(std::unique_ptr<IView> pView);

		// Add API to safely delete views
		void AddDeleteView(size_t viewIndexInList);
		void DeleteViews();

		virtual void Update(float deltaTime);

		// Dali: Temporary virtual function here, using for calling loading map from Getting GameLayer instance from
		//		 Game State Machine. We can put loading engine component code in here and loading views code in the
		//		 override function in GameLogic.LoadMap(), and have the GameLogic.LoadMap() call this parent LoadMap
		//		 inside it.
		virtual bool LoadMap(std::string&) { return true;  };

		void AddActor(Actor::Id id, std::shared_ptr<Actor> pActor);
		void DestroyActor(Actor::Id id);
		void AddDeleteActor(Actor::Id id);
		void DeleteActors();

		void ClearGame();
		void ClearSystem();

		const std::unordered_map<Actor::Id, std::shared_ptr<Actor>>& GetActors()
		{
			return m_actors;
		}

		std::shared_ptr<Actor>& GetActor(Actor::Id& actorId)
		{
			return m_actors[actorId];
		}

		ActorFactory& GetActorFactory() { return m_actorFactory; }
		ProcessManager& GetProcessManager() { return m_processManager; }
		EventManager& GetEventManager() { return m_eventManager; }
		ResourceFile& GetResourceFile() { return m_resourceFile; }
		ScriptManager& GetScriptManager() { return m_scriptManager; }
		IPhysicsManager* GetPhysicsManager() { return m_pPhysicsManager.get(); }

	protected:
		void AddPendingViews();

		virtual void RegisterWithLua();

		std::vector<std::unique_ptr<IView>> m_views;
		std::vector<std::unique_ptr<IView>> m_pendingViews;
		std::vector<size_t> m_viewDeleteList;
	
		std::unordered_map<Actor::Id, std::shared_ptr<Actor>> m_actors;
		std::vector<Actor::Id> m_unitDeleteList;

		ActorFactory m_actorFactory;
		ProcessManager m_processManager;
		EventManager m_eventManager;
		std::unique_ptr<IPhysicsManager> m_pPhysicsManager;

		Vector2f m_gravity;
		
		ResourceFile m_resourceFile;
		ScriptManager m_scriptManager;
	};
}