#pragma once
#include <ApplicationLayer.h>

class AppLogic
	: public Exelius::IGameLayer
{
public:
	AppLogic() = default;
	AppLogic(const AppLogic&) = delete;
	AppLogic& operator=(const AppLogic&) = delete;
	virtual ~AppLogic() = default;

	virtual bool Initialize() final override;
	virtual void Update(float deltaTime) final override;
	virtual const char* GetGameName() const final override { return "Sandbox"; }
};

class SandboxApp
	: public Exelius::IApplicationLayer
{
public:
	virtual ~SandboxApp() = default;
	virtual std::unique_ptr<Exelius::IGameLayer> CreateGameLayer() final override
	{
		return std::make_unique<AppLogic>();
	}
};