#include "GameLayer.h"

#include "Components/TransformComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxRenderComponent.h"
#include "Components/PowerUpComponent.h"
#include "../Components/Physics/Box2D/Box2DPhysicsComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/AIComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/TextureComponent.h"
#include "View.h"
#include <algorithm>
#include <cassert>

namespace Exelius
{
	IGameLayer::IGameLayer()
	{
		REGISTER_COMPONENT_CREATOR(TransformComponent);
		REGISTER_COMPONENT_CREATOR(BoxRenderComponent);
		REGISTER_COMPONENT_CREATOR(AnimationComponent);
		REGISTER_COMPONENT_CREATOR(PlayerComponent);
		REGISTER_COMPONENT_CREATOR(ProjectileComponent);
		REGISTER_COMPONENT_CREATOR(WallComponent);
		REGISTER_COMPONENT_CREATOR(AIComponent);
		REGISTER_COMPONENT_CREATOR(TextRenderComponent);
		REGISTER_COMPONENT_CREATOR(TextureComponent);
		REGISTER_COMPONENT_CREATOR(PowerUpComponent);
		//REGISTER_COMPONENT_CREATOR(Box2DPhysicsComponent);
		m_actorFactory.RegisterComponentCreator("Box2DPhysicsComponent",[this](Actor* pOwner)
		{
			return std::make_unique<Box2DPhysicsComponent>(pOwner, m_pPhysicsManager.get());
		});
	}

	IGameLayer::~IGameLayer()
	{
		m_views.clear();
		m_actors.clear();
	}

	bool IGameLayer::Initialize()
	{
		m_scriptManager.Initialize();

		m_gravity = { 0.0f, 0.0f };
		m_pPhysicsManager = IPhysicsManager::Create(m_gravity);
		if (!m_pPhysicsManager || !m_pPhysicsManager->Initialize())
		{
			// \todo Log an error.
			return false;
		}

		RegisterWithLua();
		AddPendingViews();

		for (auto& pView : m_views)
		{
			if (!pView->Initialize())
			{
				return false;
			}
		}
		return true;
	}

	void IGameLayer::AddView(std::unique_ptr<IView> pView)
	{
		m_pendingViews.emplace_back(std::move(pView));
	}

	void IGameLayer::AddDeleteView(size_t viewIndexInList)
	{
		m_viewDeleteList.emplace_back(viewIndexInList);
	}

	void IGameLayer::DeleteViews()
	{
		for (size_t index : m_viewDeleteList)
		{
			m_views[index] = nullptr;
		}
		// clear out delete view list
		m_viewDeleteList.clear();
	}

	void IGameLayer::Update(float deltaTime)
	{
		AddPendingViews();

		for (auto& pView : m_views)
		{
			if (pView)
				pView->UpdateInput(deltaTime);
		}

		m_eventManager.ProcessEvents();
		m_pPhysicsManager->Update(deltaTime);
		m_processManager.UpdateProcesses(deltaTime);

		for (auto& actor : m_actors)
		{
			actor.second->Update(deltaTime);
		}

		m_pPhysicsManager->HandleContacts();
		reinterpret_cast<Box2DPhysics*>(m_pPhysicsManager.get())->GetContactListener().RemoveInvalidOverlappingPair();
		m_pPhysicsManager->HandleOverlaps();
		
		DeleteViews();
		DeleteActors();

		for (auto& pView : m_views)
		{
			if (pView)
				pView->ViewScene();
		}
	}

	void IGameLayer::AddActor(Actor::Id id, std::shared_ptr<Actor> pActor)
	{
		m_actors[id] = pActor;
	}

	void IGameLayer::DestroyActor(Actor::Id id)
	{
		m_actors.erase(id);
	}

	void IGameLayer::AddDeleteActor(Actor::Id id)
	{
		bool notAlreadyExist = std::none_of(m_unitDeleteList.begin(), m_unitDeleteList.end(), [id](Actor::Id exist) { return exist == id; });
		
		if (notAlreadyExist)
		{
			m_unitDeleteList.emplace_back(id);
		}
	}

	void IGameLayer::DeleteActors()
	{
		// Loop through and delete
		std::for_each(m_unitDeleteList.begin(), m_unitDeleteList.end(), [this](Actor::Id id) {
			DestroyActor(id);
		});

		// clear out delete view list
		m_unitDeleteList.clear();
	}

	void IGameLayer::ClearGame()
	{
		ClearSystem();

		std::for_each(m_actors.begin(), m_actors.end(), [this](std::pair<const uint32_t, std::shared_ptr<Exelius::Actor>>& pair) {
			AddDeleteActor(pair.second->GetId());
		});

		std::for_each(m_views.begin(), m_views.end(), [this](std::unique_ptr<Exelius::IView>& pView) {
			if (pView)
				AddDeleteView(pView->GetIndexInViewList());
		});
	}

	void IGameLayer::ClearSystem()
	{
		m_eventManager.ResetEventManager();
		m_processManager.AbortAllProcesses();
		m_pPhysicsManager->ClearContacts();
	}

	void IGameLayer::AddPendingViews()
	{
		auto addView = [this](std::unique_ptr<IView> pView) {
			for (size_t index = 0; index < m_views.size(); ++index)
			{
				if (m_views[index] == nullptr)
				{
					pView->SetViewListIndex(index);
					m_views[index] = std::move(pView);
					return;
				}
			}
			pView->SetViewListIndex(m_views.size());
			m_views.emplace_back(std::move(pView));
		};

		for (auto& pView : m_pendingViews)
		{
			addView(std::move(pView));
		}
		m_pendingViews.clear();
	}

	void IGameLayer::RegisterWithLua()
	{
		m_scriptManager.CreateTable(); //Logic table

		m_scriptManager.CreateTable(); //Actors table
		m_scriptManager.AddToTable("Actors");

		m_scriptManager.SetGlobal("g_logic");
	}
}