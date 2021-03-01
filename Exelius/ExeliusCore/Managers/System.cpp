#include "System.h"
#include "ApplicationLayer.h"
#include "Input.h"

#include <ThirdParty/Middleware/SDL2/include/SDL.h>
#include <ThirdParty/Middleware/SDL2_ttf/include/SDL_ttf.h>
#include <Windows.h>
#include <filesystem>
#include <string>
#include <time.h>
#include <ShlObj.h>

/// <summary>
/// Exelius Engine namespace. Used for all Engine related code.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Window utilizing SDL.
	/// </summary>
	class SDLWindow
		: public IWindow
	{
	public:
		/// <summary>
		/// Constructor sets initialized state to false.
		/// </summary>
		SDLWindow()
			: m_initialized(false)
			, m_pSDLWindow(nullptr, nullptr)
		{
			//
		}

		/// <summary>
		/// Destructor Quits the SDL Video subsystem, and calls SDL_Quit;
		/// </summary>
		virtual ~SDLWindow()
		{
			m_pSDLWindow = nullptr;

			if (m_initialized)
			{
				SDL_QuitSubSystem(SDL_INIT_VIDEO);
			}
			TTF_Quit();
			SDL_Quit();
		}

		/// <summary>
		/// Starts up the SDL Video subsystem and creates the window managed by SDL.
		/// Logs any errors to the log.
		/// </summary>
		/// <param name="pName">(const char*) Name of the window.</param>
		/// <param name="width">(uint32_t) The width of the window.</param>
		/// <param name="height">(unint32_t) The height of the window.</param>
		/// <returns>(bool) True if initialized properly, false if an error occured. Error will be reported in the log.</returns>
		virtual bool Initialize(const char* pName, uint32_t width, uint32_t height) override
		{
			auto& logger = IApplicationLayer::GetInstance()->GetLogger();

			if (SDL_InitSubSystem(SDL_INIT_VIDEO))
			{
				logger.LogFatal("SDL_INIT_VIDEO has failed: ", false);
				logger.LogFatal(SDL_GetError());
				return false;
			}

			m_initialized = true;

			if (width <= 0 || height <= 0)
			{
				logger.LogFatal("Window cannot have 0 for width or height.");
				return false;
			}

			m_windowDimensions.x = width;
			m_windowDimensions.y = height;

			m_pSDLWindow = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>
				(SDL_CreateWindow(pName
					, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
					, width, height, SDL_WINDOW_OPENGL)
					, &SDL_DestroyWindow);

			if (m_pSDLWindow == nullptr)
			{
				logger.LogFatal("SDL_CreateWindow has failed: ", false);
				logger.LogFatal(SDL_GetError());
				return false;
			}

			if (TTF_Init() != 0)
			{
				logger.LogFatal("TTF_Init has failed. Error: ", false);
				logger.LogFatal(TTF_GetError());
				return false;
			}

			return true;
		}

		/// <summary>
		/// Process any input or window events that occur for this SDL window.
		/// </summary>
		/// <returns>(bool) False if the window should close.</returns>
		virtual bool ProcessEvents() const override
		{
			SDL_Event event;
			if (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					return false;
				}

				if (m_pKeyboard &&
					(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP))
				{
					GenericKeyboard::Code code = ConvertToCode(event.key.keysym.scancode);
					m_pKeyboard->SetKeyState(code, event.type == SDL_KEYDOWN);
				}
				if (m_pMouse &&
					(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP))
				{
					GenericMouse::Button button = ConvertToButton(event.button.button);
					m_pMouse->SetButtonState(button, event.type == SDL_MOUSEBUTTONDOWN);
				}

				if (m_pMouse &&
					(event.type == SDL_MOUSEWHEEL))
				{
					if (event.wheel.y > 0) // scroll up
					{
						m_pMouse->SetWheelState(GenericMouse::Direction::kWheelUp);
					}
					else if (event.wheel.y < 0) // scroll down
					{
						m_pMouse->SetWheelState(GenericMouse::Direction::kWheelDown);
					}
				}
			}

			return true;
		}

		/// <summary>
		/// Process the next frame for any input devices. Clears the input buffer.
		/// </summary>
		virtual void NextFrame() override
		{
			if (m_pKeyboard)
			{
				m_pKeyboard->NextFrame();
			}
			if (m_pMouse)
			{
				m_pMouse->NextFrame();
			}
		}

		/// <summary>
		/// Attaches a keyboard to this window.
		/// </summary>
		/// <param name="pInput">(std::unique_ptr<GenericKeyboard>) Pointer to the keyboard to attach to this window.</param>
		virtual void AttachKeyboard(std::unique_ptr<GenericKeyboard> pInput) override
		{
			m_pKeyboard = std::move(pInput);
		}

		/// <summary>
		/// Get the keyboard that is attached to this window.
		/// </summary>
		/// <returns>(GenericKeyboard*) The keyboard attached to this window.</returns>
		virtual void AttachMouse(std::unique_ptr<GenericMouse> pInput) override
		{
			m_pMouse = std::move(pInput);
		}

		/// <summary>
		/// Attaches a mouse to this window.
		/// </summary>
		/// <param name="pInput">(std::unique_ptr<GenericMouse>) Pointer to the mouse to attach to this window.</param>
		virtual GenericKeyboard* GetKeyboard() override { return m_pKeyboard.get(); }

		/// <summary>
		/// Get the mouse that is attached to this window.
		/// </summary>
		/// <returns>(GenericMouse*) The mouse attached to this window.</returns>
		virtual GenericMouse* GetMouse() override { return m_pMouse.get(); }

		/// <summary>
		/// Get the window in it's SDL format.
		/// </summary>
		/// <returns>(void*) Pointer to the window in its SDL format.</returns>
		virtual void* GetNativeWindow() const override { return m_pSDLWindow.get(); }

		virtual void Quit() final override
		{
			SDL_Event quitEvent;
			quitEvent.type = SDL_QUIT;
			SDL_PushEvent(&quitEvent);
		}

	private:
		/// <summary>
		/// Called by Process Events, converts SDL key press events from SDL Scancodes to Exelius Key Codes.
		/// </summary>
		/// <param name="key">(size_t) The key to be checked.</param>
		/// <returns>(Code) The Exelius key code pressed.</returns>
		GenericKeyboard::Code ConvertToCode(size_t key) const
		{
			if (key >= (size_t)SDL_SCANCODE_A && key <= (size_t)SDL_SCANCODE_Z)
			{
				return static_cast<GenericKeyboard::Code>
					(key - (size_t)SDL_SCANCODE_A + (size_t)GenericKeyboard::Code::kCodeA);
			}
			else if (key >= (size_t)SDL_SCANCODE_1 && key <= (size_t)SDL_SCANCODE_0)
			{
				return static_cast<GenericKeyboard::Code>
					(key - (size_t)SDL_SCANCODE_1 + (size_t)GenericKeyboard::Code::kCode1);
			}
			else if (key == (size_t)SDL_SCANCODE_SPACE)
			{
				return GenericKeyboard::Code::kCodeSpace;
			}
			else if (key == (size_t)SDL_SCANCODE_UP)
			{
				return GenericKeyboard::Code::kCodeArrowUp;
			}
			else if (key == (size_t)SDL_SCANCODE_DOWN)
			{
				return GenericKeyboard::Code::kCodeArrowDown;
			}
			else if (key == (size_t)SDL_SCANCODE_LEFT)
			{
				return GenericKeyboard::Code::kCodeArrowLeft;
			}
			else if (key == (size_t)SDL_SCANCODE_RIGHT)
			{
				return GenericKeyboard::Code::kCodeArrowRight;
			}
			else if (key == (size_t)SDL_SCANCODE_ESCAPE)
			{
				return GenericKeyboard::Code::kCodeEsc;
			}
			else if (key == (size_t)SDL_SCANCODE_LSHIFT)
			{
				return GenericKeyboard::Code::kCodeShiftLeft;
			}
			else if (key == (size_t)SDL_SCANCODE_RSHIFT)
			{
				return GenericKeyboard::Code::kCodeShiftRight;
			}
			else if (key == (size_t)SDL_SCANCODE_LALT)
			{
				return GenericKeyboard::Code::kCodeAltLeft;
			}
			else if (key == (size_t)SDL_SCANCODE_RALT)
			{
				return GenericKeyboard::Code::kCodeAltRight;
			}
			else if (key == (size_t)SDL_SCANCODE_LCTRL)
			{
				return GenericKeyboard::Code::kCodeCtrlLeft;
			}
			else if (key == (size_t)SDL_SCANCODE_RCTRL)
			{
				return GenericKeyboard::Code::kCodeCtrlRight;
			}
			else if (key == (size_t)SDL_SCANCODE_EQUALS)
			{
				return GenericKeyboard::Code::kCodeEquals;
			}
			else if (key == (size_t)SDL_SCANCODE_BACKSLASH)
			{
				return GenericKeyboard::Code::kCodeBackslash;
			}
			else if (key == (size_t)SDL_SCANCODE_BACKSPACE)
			{
				return GenericKeyboard::Code::kCodeBackspace;
			}
			else if (key == (size_t)SDL_SCANCODE_CAPSLOCK)
			{
				return GenericKeyboard::Code::kCodeCapsLock;
			}
			else if (key == (size_t)SDL_SCANCODE_COMMA)
			{
				return GenericKeyboard::Code::kCodeComma;
			}
			else if (key == (size_t)SDL_SCANCODE_DELETE)
			{
				return GenericKeyboard::Code::kCodeDelete;
			}
			else if (key == (size_t)SDL_SCANCODE_END)
			{
				return GenericKeyboard::Code::kCodeEnd;
			}
			else if (key >= (size_t)SDL_SCANCODE_F1 && key <= (size_t)SDL_SCANCODE_F12)
			{
				return static_cast<GenericKeyboard::Code>
					(key - (size_t)SDL_SCANCODE_F1 + (size_t)GenericKeyboard::Code::kCodeF1);
			}
			else if (key == (size_t)SDL_SCANCODE_HOME)
			{
				return GenericKeyboard::Code::kCodeHome;
			}
			else if (key == (size_t)SDL_SCANCODE_INSERT)
			{
				return GenericKeyboard::Code::kCodeInsert;
			}
			else if (key == (size_t)SDL_SCANCODE_LEFTBRACKET)
			{
				return GenericKeyboard::Code::kCodeBracketLeft;
			}
			else if (key == (size_t)SDL_SCANCODE_RIGHTBRACKET)
			{
				return GenericKeyboard::Code::kCodeBracketRight;
			}
			else if (key == (size_t)SDL_SCANCODE_MINUS)
			{
				return GenericKeyboard::Code::kCodeMinus;
			}
			else if (key == (size_t)SDL_SCANCODE_PAGEDOWN)
			{
				return GenericKeyboard::Code::kCodePageDown;
			}
			else if (key == (size_t)SDL_SCANCODE_PAGEUP)
			{
				return GenericKeyboard::Code::kCodePageUp;
			}
			else if (key == (size_t)SDL_SCANCODE_PERIOD)
			{
				return GenericKeyboard::Code::kCodePeriod;
			}
			else if (key == (size_t)SDL_SCANCODE_PRINTSCREEN)
			{
				return GenericKeyboard::Code::kCodePrintScreen;
			}
			else if (key == (size_t)SDL_SCANCODE_RETURN)
			{
				return GenericKeyboard::Code::kCodeEnter;
			}
			else if (key == (size_t)SDL_SCANCODE_SEMICOLON)
			{
				return GenericKeyboard::Code::kCodeSemiColon;
			}
			else if (key == (size_t)SDL_SCANCODE_SLASH)
			{
				return GenericKeyboard::Code::kCodeForwardSlash;
			}
			else if (key == (size_t)SDL_SCANCODE_TAB)
			{
				return GenericKeyboard::Code::kCodeTab;
			}
			else if (key == (size_t)SDL_SCANCODE_APOSTROPHE)
			{
				return GenericKeyboard::Code::kCodeApostrophe;
			}
			else if (key == (size_t)SDL_SCANCODE_GRAVE)
			{
				return GenericKeyboard::Code::kCodeTilde;
			}
			else if (key >= (size_t)SDL_SCANCODE_KP_1 && key <= (size_t)SDL_SCANCODE_KP_0)
			{
				return static_cast<GenericKeyboard::Code>
					(key - (size_t)SDL_SCANCODE_KP_1 + (size_t)GenericKeyboard::Code::kCodeNumPad1);
			}
			else if (key == (size_t)SDL_SCANCODE_KP_PLUS)
			{
				return GenericKeyboard::Code::kCodeNumPadPlus;
			}
			else if (key == (size_t)SDL_SCANCODE_KP_PERIOD)
			{
				return GenericKeyboard::Code::kCodeNumPadPeriod;
			}
			else if (key == (size_t)SDL_SCANCODE_KP_MINUS)
			{
				return GenericKeyboard::Code::kCodeNumPadMinus;
			}
			else if (key == (size_t)SDL_SCANCODE_KP_MULTIPLY)
			{
				return GenericKeyboard::Code::kCodeNumPadMultiply;
			}
			else if (key == (size_t)SDL_SCANCODE_KP_DIVIDE)
			{
				return GenericKeyboard::Code::kCodeNumPadDivide;
			}
			else if (key == (size_t)SDL_SCANCODE_KP_ENTER)
			{
				return GenericKeyboard::Code::kCodeNumPadEnter;
			}

			return GenericKeyboard::Code::kCodeMax;
		}

		/// <summary>
		/// Called by Process Events, converts SDL mouse press events from SDL codes to Exelius Button Codes.
		/// </summary>
		/// <param name="key">(size_t) The key to be checked.</param>
		/// <returns>(Button) The Exelius button code pressed.</returns>
		GenericMouse::Button ConvertToButton(size_t button) const
		{
			static const size_t kSDLLeftButton = SDL_BUTTON_LEFT;
			static const size_t kSDLMiddleButton = SDL_BUTTON_MIDDLE;
			static const size_t kSDLRightButton = SDL_BUTTON_RIGHT;

			if (button == kSDLLeftButton)
			{
				return GenericMouse::Button::kButtonLeft;
			}
			else if (button == kSDLRightButton)
			{
				return GenericMouse::Button::kButtonRight;
			}
			else if (button == kSDLMiddleButton)
			{
				return GenericMouse::Button::kButtonMiddle;
			}
			return GenericMouse::Button::kButtonMax;
		}

		bool m_initialized;
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_pSDLWindow;
		std::unique_ptr<GenericKeyboard> m_pKeyboard;
		std::unique_ptr<GenericMouse> m_pMouse;
	};

	/// <summary>
	/// A Windows specific implimentation of the ISystem interface.
	/// Works for both Win32 and Win64 platforms.
	/// </summary>
	class WindowsSystem
		: public ISystem
	{
	public:
		/// <summary>
		/// Windows specific recursive file retrieval.
		/// </summary>
		/// <param name="path">Path to files to retrieve.</param>
		/// <returns>Files retreived.</returns>
		virtual std::vector<std::string> GetDirectoryFiles(const std::string& path) final override
		{
			std::vector<std::string> files;
			GetFilesRecursive(path, "", files);
			return files;
		}
	private:
		/// <summary>
		/// Recursively store all the files in a given directory.
		/// </summary>
		/// <param name="path">Path to directory.</param>
		/// <param name="directory">Directory to retrieve.</param>
		/// <param name="files">The files retrieved.</param>
		void GetFilesRecursive(const std::string& path, const std::string& directory, std::vector<std::string>& files)
		{
			std::string fullPath = path + "/" + directory + "/*";
			WIN32_FIND_DATAA findData;
			HANDLE findHandle = FindFirstFileA(fullPath.c_str(), &findData);
			if (findHandle == INVALID_HANDLE_VALUE)
			{
				return;
			}

			std::string rootDirectory = directory;
			if (!rootDirectory.empty())
			{
				rootDirectory += "/";
			}

			while (true)
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					std::string directoryName = findData.cFileName;
					if (directoryName != "." && directoryName != "..")
					{
						std::string nextDirectory = rootDirectory + directoryName;
						GetFilesRecursive(path, nextDirectory, files);
					}
				}
				else
				{
					files.emplace_back(rootDirectory + findData.cFileName);
				}

				bool result = FindNextFileA(findHandle, &findData);
				if (!result)
					break;
			}
			FindClose(findHandle);
		}

		/// <summary>
		/// Gets "Windows" as the current Operating System.
		/// Retrieved value gets logged, then returned.
		/// </summary>
		/// <returns>(const char*) C-Style string containing the name "Windows".</returns>
		virtual const char* GetSystemName() const override
		{
			//auto& logger = IApplicationLayer::GetInstance()->GetLogger();

			//logger.LogInfo("System Name: ", false);
			//logger.LogInfo("Windows");

			return "Windows";
		}

		/// <summary>
		/// Gets the number of cores\processors available on the system.
		/// Retrieved value gets logged, then returned.
		/// </summary>
		/// <returns>(unsigned int) Integer containing the number of cores available.</returns>
		virtual unsigned int GetNumberCores() const override
		{
			SYSTEM_INFO sysInfo;
			auto& logger = IApplicationLayer::GetInstance()->GetLogger();

			GetSystemInfo(&sysInfo);
			logger.LogInfo("Number of Cores: ", false);
			logger.LogInfo(std::to_string(sysInfo.dwNumberOfProcessors).c_str());

			return sysInfo.dwNumberOfProcessors;
		}

		/// <summary>
		/// Gets the total physical memory/RAM that is installed on the system. Returns 0 if unsuccessful.
		/// Retrieved value gets logged, then returned.
		/// </summary>
		/// <returns>(size_t) Total number of *Gigabytes* installed on the system.</returns>
		virtual size_t GetTotalMemory() const override
		{
			MEMORYSTATUSEX buffer;
			buffer.dwLength = sizeof(buffer);
			auto& logger = IApplicationLayer::GetInstance()->GetLogger();

			if (!GlobalMemoryStatusEx(&buffer))
			{
				logger.LogDebug("Failed to get memory information: ", false);
				logger.LogDebug(std::to_string(GetLastError()).c_str());
			}

			logger.LogInfo("Total Memory: ", false);
			logger.LogInfo(std::to_string(((size_t)buffer.ullTotalPhys) / (1024 * 1024 * 1024) + 1).c_str(), false);
			logger.LogInfo(" Gb");

			//Return SystemMemoryInfo.
			return (((size_t)buffer.ullTotalPhys) / (1024 * 1024 * 1024)) + 1;
		}

		/// <summary>
		/// Gets the amount of free disk space available on the selected drive. Returns 0 if unsuccessful.
		/// If no parameter is entered, it will default to the projects root directory.
		/// Retrieved value gets logged, then returned.
		/// </summary>
		/// <param name="pDirectory">(const char*) Disk partition directory. Default is nullptr.</param>
		/// <returns>(unsigned long long) Integer containing the total space on the partition in *kilobytes*.</returns>
		virtual unsigned long long GetFreeDiskSpace(const char* pDirectory) const override
		{
			ULARGE_INTEGER freeSpace;
			auto& logger = IApplicationLayer::GetInstance()->GetLogger();

			if (GetDiskFreeSpaceExA(pDirectory, NULL, NULL, &freeSpace) == 0)
			{
				logger.LogDebug("Failed to get disk free space: ", false);
				logger.LogDebug(std::to_string(GetLastError()).c_str());
			}

			logger.LogInfo("Free Disk Space: ", false);
			logger.LogInfo(std::to_string((freeSpace.QuadPart) / 1024).c_str(), false);
			logger.LogInfo(" kb");

			return (freeSpace.QuadPart) / 1024;
		}

		/// <summary>
		/// Recursively logs all of a directories contents including subdirectories.
		/// </summary>
		/// <param name="pDirectory">(const char*) The path of the directory to log.</param>
		virtual void LogDirectoryContents(const char* pDirectory) const override
		{
			//Followed guide from link:
			//https://thispointer.com/c-get-the-list-of-all-files-in-a-given-directory-and-its-sub-directories-using-boost-c17/

			//Create the directory iterator.
			std::filesystem::recursive_directory_iterator iter(pDirectory);
			std::filesystem::recursive_directory_iterator end;

			auto& logger = IApplicationLayer::GetInstance()->GetLogger();

			//For all contents of the given directory...
			while (iter != end)
			{
				//Log the iterators path to the console.
				logger.LogInfo("Filepath: ", false);
				logger.LogInfo(iter->path().string().c_str());

				std::error_code ec;

				//Increment the iterator to point to next entry in recursive iteration.
				iter.increment(ec);

				//If a directory cannot be accessed...
				if (ec)
				{
					logger.LogDebug("Error while accessing filepath: ", false);
					logger.LogDebug(iter->path().string().c_str(), false);
					logger.LogDebug(" : ", false);
					logger.LogDebug(ec.message().c_str());
				}
			}
		}

		/// <summary>
		/// Get the time on the system.
		/// </summary>
		/// <returns>(const char*) The time on the system in the format: [H:M:S]</returns>
		virtual const char* GetSystemTime() const override
		{
			//Followed guide from user JLBorges (http://www.cplusplus.com/forum/general/189594/)
			//Combined with guide from user kerrytazi (https://stackoverflow.com/questions/52728952/how-do-i-write-a-message-timestamp-to-a-log-file)

			static char timeString[32];

			auto timePoint = std::chrono::system_clock::now();
			std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);

			struct tm timeInfo;

			// localtime_s, Microsoft version (returns zero on success, an error code on failure)
			if (localtime_s(&timeInfo, &currentTime) == 0)
			{
				strftime(timeString, sizeof(timeString), "[%H:%M:%S]", &timeInfo);
			}

			return timeString;
		}

		/// <summary>
		/// Get the date on the system.
		/// </summary>
		/// <returns>(const char*) The date on the system in the format: Y_m_d (Including underscores).</returns>
		virtual const char* GetSystemDate() const override
		{
			//Followed guide from user JLBorges (http://www.cplusplus.com/forum/general/189594/)
			//Combined with guide from user kerrytazi (https://stackoverflow.com/questions/52728952/how-do-i-write-a-message-timestamp-to-a-log-file)

			static char timeString[32];

			auto timePoint = std::chrono::system_clock::now();
			std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);

			struct tm timeInfo;

			// localtime_s, Microsoft version (returns zero on success, an error code on failure)
			if (localtime_s(&timeInfo, &currentTime) == 0)
			{
				strftime(timeString, sizeof(timeString), "%Y_%m_%d", &timeInfo);
			}

			return timeString;
		}

		/// <summary>
		/// Checks if the input file exists in the provided directory.
		/// If no directory is provided it will check the Game Directory.
		/// </summary>
		/// <param name="pFilename">(const char*) Name of the file to search for.</param>
		/// <param name="pDirectory">(const cahr*) Path of the directory to search.</param>
		/// <returns>(bool) True if the file was found, false if not.</returns>
		virtual bool DoesFileExist(const char* pFilename, const char* pDirectory) const override
		{
			std::filesystem::path dirPath;

			//If there is not a directory provided..
			if (pDirectory == nullptr)
			{
				dirPath = std::filesystem::current_path();
			}
			else
			{
				dirPath = pDirectory;
			}

			std::string comparingFilepath = dirPath.string();
			comparingFilepath += "\\";
			comparingFilepath += pFilename;

			//Create the directory iterator.
			std::filesystem::recursive_directory_iterator iter(dirPath);
			std::filesystem::recursive_directory_iterator end;

			//For all contents of the given directory...
			while (iter != end)
			{
				if (std::strcmp(iter->path().string().c_str(), comparingFilepath.c_str()) == 0)
				{
					return true;
				}

				std::error_code ec;

				//Increment the iterator to point to next entry in recursive iteration.
				iter.increment(ec);

				//If a directory cannot be accessed...
				if (ec)
				{
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Creates new directory with given name at given path.
		/// If no path is given, it will use the projects current directory.
		/// If no filename is given, "New Folder" will be used.
		/// This function is used in the initialization of the Logging system, so there is no log output upon failure.
		/// </summary>
		/// <param name="pPath">(const char*) Path to the directory to create.</param>
		/// <param name="pName">(const char*) Name of the directory to create.</param>
		/// <returns>(bool) False if creating the directory fails.</returns>
		virtual bool SystemCreateDirectory(const char* pPath, const char* pName) const override
		{
			std::filesystem::path newDirectory;
			if (pPath)
			{
				newDirectory = pPath;
			}
			else
			{
				newDirectory = std::filesystem::current_path();
			}
			newDirectory += "\\";

			if (pName)
			{
				newDirectory += pName;
			}
			else
			{
				newDirectory += "New Folder";
			}

			if (CreateDirectoryA(newDirectory.string().c_str(), nullptr) == 0)
			{
				return false;
			}

			return true;
		}

		/// <summary>
		/// Get the user directory.
		/// </summary>
		/// <returns>(const char*) The filepath of the user directory for the current user.</returns>
		virtual const char* GetUserDirectory() const override
		{
			//Guide followed by user guestgulkan from link:
			//http://www.cplusplus.com/forum/general/39766/
			//Guide followed by user Phillip from link:
			//https://stackoverflow.com/questions/9542611/how-to-get-the-current-users-home-directory-in-windows

			auto& logger = IApplicationLayer::GetInstance()->GetLogger();
			WCHAR userPath[260];
			static char charPath[260];
			if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userPath)))
			{
				char DefChar = ' ';
				WideCharToMultiByte(CP_ACP, 0, userPath, -1, charPath, 255, &DefChar, nullptr);
			}
			logger.LogDebug(charPath);
			return charPath;
		}

		/// <summary>
		/// Get this projects directory.
		/// </summary>
		/// <returns>(const char*) The filepath of this projects directory.</returns>
		virtual const char* GetProjectDirectory() const override
		{
			std::filesystem::path directory;
			directory = std::filesystem::current_path();
			static char charPath[260];
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP, 0, directory.c_str(), -1, charPath, 255, &DefChar, nullptr);
			return charPath;
		}

		virtual void SetCurrentWorkingDirectory(const std::string& path) override
		{
			SetCurrentDirectoryA(path.c_str());
		}

		/// <summary>
		/// Creates a window using platform specific API.
		/// </summary>
		/// <returns>(std::unique_ptr<IWindow>) Returns a unique pointer to the window.</returns>
		virtual std::unique_ptr<IWindow> CreateSystemWindow(const char* pName, uint32_t width, uint32_t height) override
		{
			std::unique_ptr<IWindow> pWindow = std::make_unique<SDLWindow>();

			if (pWindow->Initialize(pName, width, height))
			{
				return pWindow;
			}

			return nullptr;
		}
	};

	/// <summary>
	/// Creates the pointer to a platform specific implimentation of this class.
	/// </summary>
	/// <returns>Returns a unique pointer.</returns>
	std::unique_ptr<ISystem> ISystem::Create()
	{
		#if defined(_WIN32)
			return std::make_unique<WindowsSystem>();
		#else
			return nullptr;
		#endif
	}
}