#include "Application.h"
//#include "View/TestView.h"
#include "View/GeneratorView.h"

bool AppLogic::Initialize()
{
	auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

	auto pSystem = Exelius::IApplicationLayer::GetInstance()->GetSystem();
	pSystem->SetCurrentWorkingDirectory(pSystem->GetProjectDirectory());
	m_resourceFile.Load("Game/Resources/resource.bin");

	bool result = Exelius::IGameLayer::Initialize();
	if (!result)
	{
		logger.LogFatal("GameLayer failed to initialize.");
		return result;
	}

	std::string testActor = "Actors/Player.xml";

	auto pActor = m_actorFactory.CreateActor(m_resourceFile.LoadResource(testActor.c_str()));

	Exelius::IView* pView = new GeneratorView(pActor);


	if (pView && pView->Initialize())
	{
		AddView(std::move(std::unique_ptr<Exelius::IView>(pView)));
	}
	m_actors[pActor->GetId()] = std::move(pActor);

	return true;
}

void AppLogic::Update(float deltaTime)
{
	Exelius::IGameLayer::Update(deltaTime);
}