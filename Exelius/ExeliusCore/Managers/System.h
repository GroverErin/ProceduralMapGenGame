#pragma once
#include "Utilities/Vector2.h"

#include <memory>
#include <vector>
#include <string>

/// <summary>
/// Exelius Engine namespace. Used for all Engine related code.
/// </summary>
namespace Exelius
{
	class GenericKeyboard;
	class GenericMouse;

	/// <summary>
	/// Interface for windows. Handles the creation, destruction, window events, and input events attached to a window.
	/// </summary>
	class IWindow
	{
	public:
		/// <summary>
		/// Virtual Window Destructor.
		/// </summary>
		virtual ~IWindow() = default;

		/// <summary>
		/// Handles the creation of the window. Logs any errors.
		/// </summary>
		/// <param name="pName">(const char*) The name of the window being created.</param>
		/// <param name="width">(uint32_t) The width of the window.</param>
		/// <param name="height">(unint32_t) The height of the window.</param>
		/// <returns>(bool) True if initialized properly, false if an error occured. Error will be reported in the log.</returns>
		virtual bool Initialize(const char* pName, uint32_t width, uint32_t height) = 0;

		/// <summary>
		/// Gets the dimesions of the window in a Vector2. X is Width, Y is Height.
		/// </summary>
		/// <returns>(Vector2u) Vector2 containing the width and height of the window.</returns>
		virtual Vector2u GetWindowDimensions() { return m_windowDimensions; }

		/// <summary>
		/// Process any input or window events that occur for this window.
		/// </summary>
		/// <returns>(bool) False if the window should close.</returns>
		virtual bool ProcessEvents() const = 0;

		/// <summary>
		/// Process the next frame for any input devices. Clears the input buffer.
		/// </summary>
		virtual void NextFrame() = 0;

		/// <summary>
		/// Attaches a keyboard to this window.
		/// </summary>
		/// <param name="pInput">(std::unique_ptr<GenericKeyboard>) Pointer to the keyboard to attach to this window.</param>
		virtual void AttachKeyboard(std::unique_ptr<GenericKeyboard> pInput) = 0;

		/// <summary>
		/// Get the keyboard that is attached to this window.
		/// </summary>
		/// <returns>(GenericKeyboard*) The keyboard attached to this window.</returns>
		virtual GenericKeyboard* GetKeyboard() = 0;

		/// <summary>
		/// Attaches a mouse to this window.
		/// </summary>
		/// <param name="pInput">(std::unique_ptr<GenericMouse>) Pointer to the mouse to attach to this window.</param>
		virtual void AttachMouse(std::unique_ptr<GenericMouse> pInput) = 0;

		/// <summary>
		/// Get the mouse that is attached to this window.
		/// </summary>
		/// <returns>(GenericMouse*) The mouse attached to this window.</returns>
		virtual GenericMouse* GetMouse() = 0;

		/// <summary>
		/// Get the window in it's native format. (SDL, SFML, etc)
		/// </summary>
		/// <returns>(void*) Pointer to the window in its native format.</returns>
		virtual void* GetNativeWindow() const = 0;

		virtual void Quit() = 0;

	protected:
		Vector2u m_windowDimensions;
	};

	/// <summary>
	/// Interface for systems. Handles any platform specific code i.e. Windows, Linux.
	/// </summary>
	class ISystem
	{
	public:
		/// <summary>
		/// Virtual System Destructor.
		/// </summary>
		virtual ~ISystem() = default;

		/// <summary>
		/// Gets the name of the current Operating System.
		/// </summary>
		/// <returns>(const char*) C-Style string containing the name of the Operating System.</returns>
		virtual const char* GetSystemName() const = 0;

		/// <summary>
		/// Gets the number of cores\processors available on the system.
		/// </summary>
		/// <returns>(unsigned int) Integer containing the number of cores available.</returns>
		virtual unsigned int GetNumberCores() const = 0;

		/// <summary>
		/// Gets the total physical memory/RAM that is installed on the system.
		/// </summary>
		/// <returns>(size_t) Total number of *bytes* installed on the system.</returns>
		virtual size_t GetTotalMemory() const = 0;

		/// <summary>
		/// Gets the amount of free disk space available on the selected drive.
		/// If no parameter is entered, it will default to the projects root directory.
		/// </summary>
		/// <param name="pDirectory">(const char*) Disk partition directory. Default is nullptr.</param>
		/// <returns>(unsigned long long) Integer containing the total space on the partition in *bytes*.</returns>
		virtual unsigned long long GetFreeDiskSpace(const char* pDirectory = nullptr) const = 0;

		/// <summary>
		/// Recursively logs the contents of the input directory.
		/// </summary>
		/// <param name="pDirectory">(const char*) Directory to log.</param>
		virtual void LogDirectoryContents(const char* pDirectory) const = 0;

		/// <summary>
		/// Gets all of the files in a given directory recursively.
		/// </summary>
		/// <param name="path">The directory to get the files from.</param>
		/// <returns>The files in the directory.</returns>
		virtual std::vector<std::string> GetDirectoryFiles(const std::string& path) = 0;

		/// <summary>
		/// Get the time on the system.
		/// </summary>
		/// <returns>(const char*) The time on the system in the format: [H:M:S]</returns>
		virtual const char* GetSystemTime() const = 0;

		/// <summary>
		/// Get the date on the system.
		/// </summary>
		/// <returns>(const char*) The date on the system in the format: Y_m_d (Including underscores).</returns>
		virtual const char* GetSystemDate() const = 0;

		/// <summary>
		/// Checks if the input file exists in the provided directory.
		/// If no directory is provided it will check the Game Directory.
		/// </summary>
		/// <param name="pFilename">(const char*) Name of the file to search for.</param>
		/// <param name="pDirectory">(const cahr*) Path of the directory to search.</param>
		/// <returns>(bool) True if the file was found, false if not.</returns>
		virtual bool DoesFileExist(const char* pFilename, const char* pDirectory = nullptr) const = 0;

		/// <summary>
		/// Creates new directory with given name at given path.
		/// If no path is given, it will use the projects current directory.
		/// If no filename is given, "New Folder" will be used.
		/// This function is used in the initialization of the Logging system, so there is no log output upon failure.
		/// </summary>
		/// <param name="pPath">(const char*) Path to the directory to create.</param>
		/// <param name="pName">(const char*) Name of the directory to create.</param>
		/// <returns>(bool) False if creating the directory fails.</returns>
		virtual bool SystemCreateDirectory(const char* pPath = nullptr, const char* pName = nullptr) const = 0;

		/// <summary>
		/// Get the user directory.
		/// </summary>
		/// <returns>(const char*) The filepath of the user directory for the current user.</returns>
		virtual const char* GetUserDirectory() const = 0;

		/// <summary>
		/// Get this projects directory.
		/// </summary>
		/// <returns>(const char*) The filepath of this projects directory.</returns>
		virtual const char* GetProjectDirectory() const = 0;

		/// <summary>
		/// 
		/// </summary>
		virtual void SetCurrentWorkingDirectory(const std::string& path) = 0;

		/// <summary>
		/// Creates a window using platform specific API.
		/// </summary>
		/// <returns>(std::unique_ptr<IWindow>) Returns a unique pointer to the window.</returns>
		virtual std::unique_ptr<IWindow> CreateSystemWindow(const char* pName, uint32_t width, uint32_t height) = 0;

		/// <summary>
		/// Creates the pointer to a platform specific implimentation of this class.
		/// </summary>
		/// <returns>(std::unique_ptr<ISystem>) Returns a unique pointer to the platform specifc class.</returns>
		static std::unique_ptr<ISystem> Create();
	};
}