#include <string>
#include <chrono>

#include "ApplicationLayer.h"
#include "Managers/Input.h"

namespace Exelius
{
	IApplicationLayer* IApplicationLayer::s_pAppLayer = nullptr;

	/// <summary>
	/// Initializes the base Application layer, warming up the various systems used by the application.
	/// Sets the size of the window to be created.
	/// *Must* be called first.
	/// </summary>
	/// <param name="windowWidth">(uint32_t) The width of the window.</param>
	/// <param name="windowHeight">(uint32_t) The height of the window.</param>
	/// <returns>True if the initialization succeeded, false otherwise.</returns>
	bool IApplicationLayer::Initialize(uint32_t windowWidth, uint32_t windowHeight)
	{
		//--------------------System and Log Creation--------------------
		s_pAppLayer = this;
		m_pSystem = ISystem::Create();

		if (!m_logger.Initialize(Logger::Level::kLevelInfo, m_pSystem.get()))
		{
			//Logging is CRITICAL, so if intialization fails, exit the engine.
			return false;
		}

		m_logger.LogInfo("Initializing IApplicationLayer.");

		//--------------------GameLayer Creation--------------------
		m_pGameLayer = CreateGameLayer();
		if (m_pGameLayer == nullptr)
		{
			m_logger.LogDebug("CreateGameLayer() returned nullptr.");
			return false;
		}

		//--------------------Window Creation--------------------
		m_pWindow = m_pSystem->CreateSystemWindow(m_pGameLayer->GetGameName(), windowWidth, windowHeight);
		if (m_pWindow == nullptr)
		{
			m_logger.LogFatal("CreateSystemWindow() returned nullptr.");
			return false;
		}

		//--------------------Graphics Creation--------------------
		m_pGraphics = IGraphics::Create();
		if (m_pGraphics == nullptr)
		{
			m_logger.LogFatal("Graphics Create() returned nullptr.");
			return false;
		}

		if (!m_pGraphics->Initialize(m_pWindow.get()))
		{
			m_logger.LogFatal("Graphics failed to initialize.");
			return false;
		}

		//--------------------Keyboard Creation--------------------
		auto pKeyBoard = GenericKeyboard::Create();
		if (pKeyBoard->Initialize())
		{
			m_pWindow->AttachKeyboard(std::move(pKeyBoard));
		}
		else
		{
			m_logger.LogFatal("Keyboard Attachment Failed.");
			return false;
		}

		//--------------------Mouse Creation--------------------

		auto pMouse = GenericMouse::Create();
		if (pMouse->Initialize())
		{
			m_pWindow->AttachMouse(std::move(pMouse));
		}
		else
		{
			m_logger.LogFatal("Mouse Attachment Failed.");
			return false;
		}

		//--------------------Audio Creation--------------------

		m_pAudio = IAudio::Create();
		if (m_pAudio == nullptr)
		{
			m_logger.LogFatal("Audio Create() returned nullptr.");
			return false;
		}

		if (!m_pAudio->Initialize())
		{
			m_logger.LogFatal("Audio failed to initialize.");
			return false;
		}

		//--------------------GameLayer Initialization--------------------

		if (!m_pGameLayer->Initialize())
		{
			m_logger.LogFatal("Failed to initialize GameLayer.");
			return false;
		}

		return true;
	}

	/// <summary>
	/// Run the gamelayer and update the window.
	/// </summary>
	void IApplicationLayer::Run()
	{
		m_logger.LogInfo("Running ApplicationLayer.");

		auto previousTime = std::chrono::high_resolution_clock::now();
		while (m_pWindow->ProcessEvents())
		{
			auto time = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaTime = time - previousTime;
			m_pGameLayer->Update(deltaTime.count());
			m_pWindow->NextFrame();
			previousTime = time;
		}
	}

	/// <summary>
	/// Close the systems used by the application.
	/// </summary>
	void IApplicationLayer::Shutdown()
	{
		m_logger.LogInfo("Shutting Down ApplicationLayer.");

		m_pAudio = nullptr;
	}
}