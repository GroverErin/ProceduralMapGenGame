// Exelius Game Engine
// Created By: Erin Grover
// File Last Edited: 9/15/2020
// Last Edit: Improvements in commenting with Doxygen formatting support.

#pragma once
#include "Managers/System.h"
#include "Game/GameLayer.h"
#include "Utilities/Logger.h"
#include "Managers/Graphics.h"
#include "Managers/Audio.h"

/// <summary>
/// Exelius Engine namespace. Used for all Engine related code.
/// </summary>
namespace Exelius
{
	class IGameLayer;
	class GenericKeyboard;
	class GenericMouse;

	/// <summary>
	/// The interface for applications. All user applications must use this API in order to function.
	/// User applications *must* call the Initialize function before running. Static Singleton Class.
	/// 
	/// \b Example:
	/// ~~~~~
	/// //Create client-defined application that inherets from IApplicationLayer.
	/// GameApplication app;
	/// if (!app.Initialize())
	/// {
	///		return 1; //Return failure code.
	/// }
	/// app.Run();
	/// app.Shutdown();
	/// return 0; //Return success code.
	/// ~~~~~
	/// </summary>
	class IApplicationLayer
	{
	public:

		/// <summary>
		/// Calls the destructor of derived Application classes.
		/// </summary>
		virtual ~IApplicationLayer() = default;

		/// <summary>
		/// IApplicationLayer is a singleton. This retrieves the instance of that singleton.
		/// </summary>
		/// <returns>The pointer to the application layer instance.</returns>
		static IApplicationLayer* GetInstance() { return s_pAppLayer; }

		/// <summary>
		/// Initializes the base Application layer, warming up the various systems used by the application.
		/// Sets the size of the window to be created.
		/// *Must* be called first.
		/// </summary>
		/// <param name="windowWidth">(uint32_t) The width of the window.</param>
		/// <param name="windowHeight">(uint32_t) The height of the window.</param>
		/// <returns>True if the initialization succeeded, false otherwise.</returns>
		bool Initialize(uint32_t windowWidth, uint32_t windowHeight);

		/// <summary>
		/// Run the gamelayer and update the window.
		/// </summary>
		void Run();

		/// <summary>
		/// Close the systems used by the application.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// Calls the clients game layer creation code.
		/// </summary>
		/// <returns>A pointer to the clients gamelayer.</returns>
		virtual std::unique_ptr<IGameLayer> CreateGameLayer() = 0;

		/// <summary>
		/// Retrieve the client gamelayer.
		/// </summary>
		/// <returns>A pointer to the clients gamelayer.</returns>
		IGameLayer* GetGameLayer() const
		{
			return m_pGameLayer.get();
		}

		/// <summary>
		/// Retrieve the system manager from the Engine.
		/// </summary>
		/// <returns>A pointer to the system.</returns>
		ISystem* GetSystem() const
		{
			return m_pSystem.get();
		}

		/// <summary>
		/// Retrieve the graphics layer manager from the Engine.
		/// </summary>
		/// <returns>A pointer to the graphics.</returns>
		IGraphics* GetGraphics() const
		{
			return m_pGraphics.get();
		}

		const std::unique_ptr<IGraphics>& GetGraphicsRef()
		{
			return m_pGraphics;
		}

		/// <summary>
		/// Retrieve the keyboard input manager from the Engine.
		/// </summary>
		/// <returns>A pointer to the current keyboard interface.</returns>
		GenericKeyboard* GetKeyboardInput() const
		{
			return m_pWindow->GetKeyboard();
		}

		/// <summary>
		/// Retrieve the mouse input manager from the Engine.
		/// </summary>
		/// <returns>A pointer to the current mouse interface.</returns>
		GenericMouse* GetMouseInput() const
		{
			return m_pWindow->GetMouse();
		}

		/// <summary>
		/// Retrieve the audio layer manager from the Engine.
		/// </summary>
		/// <returns>A pointer to the audio.</returns>
		IAudio* GetAudio() const
		{
			return m_pAudio.get();
		}
		
		/// <summary>
		/// Retrieve the logging system from the engine.
		/// </summary>
		/// <returns>A reference to the logging system.</returns>
		Logger& GetLogger()
		{
			return m_logger;
		}

		IWindow* GetWindow() const
		{
			return m_pWindow.get();
		}

	private:
		static IApplicationLayer* s_pAppLayer;

		std::unique_ptr<ISystem> m_pSystem;
		std::unique_ptr<IWindow> m_pWindow;

		std::unique_ptr<IGraphics> m_pGraphics;
		std::unique_ptr<IAudio> m_pAudio;

		std::unique_ptr<IGameLayer> m_pGameLayer;

		Logger m_logger;
	};
}