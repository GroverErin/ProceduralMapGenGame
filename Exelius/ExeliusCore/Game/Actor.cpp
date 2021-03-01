#include "Actor.h"
#include "ApplicationLayer.h"

bool Exelius::Actor::Initialize(tinyxml2::XMLElement* pData)
{
	std::string name(pData->Name());
	if (name != "Actor")
	{
		return false;
	}
	return true;
}

bool Exelius::Actor::PostInit()
{
	for (auto& pair : m_components)
	{
		if (!pair.second->PostInit())
		{
			return false;
		}
	}
	return true;
}

void Exelius::Actor::RegisterWithScript()
{
	for (auto& pair : m_components)
	{
		pair.second->RegisterWithScript();
	}
}

void Exelius::Actor::Destroy()
{
	auto gameLayer = Exelius::IApplicationLayer::GetInstance()->GetGameLayer();
	gameLayer->DestroyActor(m_id);
}

void Exelius::Actor::Update(float deltaTime)
{
	for (auto& pair : m_components)
	{
		pair.second->Update(deltaTime);
	}
}

void Exelius::Actor::Render(const std::unique_ptr<Exelius::IGraphics>& graphics)
{
	for (auto& pair : m_components)
	{
		pair.second->Render(graphics);
	}
}

void Exelius::Actor::AddComponent(std::unique_ptr<IActorComponent> pComponent)
{
	if (pComponent != nullptr)
	{
		m_components[pComponent->GetId()] = move(pComponent);
	}
	else
	{
		auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();
		logger.LogWarning("Unable to add component; Component was null");
	}
}

Exelius::IActorComponent* Exelius::Actor::GetComponent(IActorComponent::Id id)
{
	auto itr = m_components.find(id);
	if (itr != m_components.end())
	{
		return itr->second.get();
	}
	return nullptr;
}

std::shared_ptr<Exelius::Actor> Exelius::ActorFactory::CreateActor(std::shared_ptr<Resource> pResource)
{
	auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.Parse(pResource->GetData().data(), pResource->GetData().size());

	logger.LogInfo("Attempting to create Actor from file '", false);
	logger.LogSevere(pResource->GetName().c_str(), false);
	logger.LogInfo("'");

	if (error != tinyxml2::XML_SUCCESS)
	{
		logger.LogSevere("Unable to load file: ", false);
		logger.LogSevere(pResource->GetName().c_str());
		logger.LogSevere(tinyxml2::XMLDocument::ErrorIDToName(error));
		return nullptr;
	}

	std::shared_ptr<Actor> pActor(new Actor(m_nextActorId));
	m_nextActorId++;

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement();
	if (!pActor->Initialize(pRoot))
	{
		logger.LogSevere("Unable to initialize actor: ", false);
		logger.LogSevere(pRoot->Name());
		return nullptr;
	}

	for (tinyxml2::XMLElement* pElement = pRoot->FirstChildElement(); pElement;
		pElement = pElement->NextSiblingElement())
	{
		pActor->AddComponent(std::move(CreateComponent(pElement, pActor.get())));
	}

	if (!pActor->PostInit())
	{
		logger.LogSevere("Unable to post init actor: ", false);
		logger.LogSevere(pRoot->Name());
		return nullptr;
	}

	return pActor;
}

std::unique_ptr<Exelius::IActorComponent> Exelius::ActorFactory::CreateComponent(tinyxml2::XMLElement* pData, Actor* pOwner)
{
	auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

	const char* pName = pData->Name();
	std::unique_ptr<IActorComponent> pComponent;
	auto createItr = m_actorComponentCreatorMap.find(pName);

	logger.LogInfo("Attempting to create component '", false);
	logger.LogSevere(pName, false);
	logger.LogInfo("'");

	if (createItr != m_actorComponentCreatorMap.end())
	{
		pComponent = move(createItr->second(pOwner));
	}
	else
	{
		logger.LogSevere("Unable to find creator for component : ", false);
		logger.LogSevere(pName);
		return nullptr;
	}

	if (pComponent != nullptr && !pComponent->Initialize(pData))
	{
		return nullptr;
	}

	return pComponent;
}
