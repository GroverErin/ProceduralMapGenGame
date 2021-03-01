#pragma once
#include <fstream>

//Exelius Engine namespace. Used for all Engine related code.
namespace Exelius
{
	class ISystem;

	/// <summary>
	/// Logging class used to write to an output log after every run of the engine.
	/// \todo There is a lot of copy-paste code here. Fix it.
	/// </summary>
	class Logger
	{
	public:
		/// <summary>
		/// Scoped Enum for the different levels of severity the logger can output.
		/// </summary>
		enum class Level
		{
			kLevelInfo,
			kLevelDebug,
			kLevelWarning,
			kLevelSevere,
			kLevelFatal,

			//Setting level to this will not display any logging information.
			kLevelNone
		};

		/// <summary>
		/// Initializes the logger by seleting an output path and folder name, and logging severity.
		/// </summary>
		/// <param name="level">(Level) Severity to log. Will log anything just as and more severe as this level.</param>
		/// <returns>(bool) True if initialize is a success, false if there was an error. Error will not be logged (since the logger failed to be created).</returns>
		bool Initialize(Level level = Level::kLevelInfo, ISystem* pSystem = nullptr);

		/// <summary>
		/// Sets the logging level to the input level.
		/// </summary>
		/// <param name="level">(Level) New logging level to set the logger to.</param>
		void SetLogLevel(Level level) { m_logLevel = level; }

		/// <summary>
		/// Get the current logging level.
		/// </summary>
		/// <returns>(Level) The current logging level.</returns>
		Level GetLogLevel() const { return m_logLevel; }

		/// <summary>
		/// Logs a message with the severity of Information. This is the lowest severity.
		/// </summary>
		/// <param name="pMessage">(const char*) The message to print.</param>
		/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
		void LogInfo(const char* pMessage, bool newLine = true);

		/// <summary>
		/// Logs a message with the severity of Information. This is the second lowest severity and should be used for debugging purposes.
		/// </summary>
		/// <param name="pMessage">(const char*) The message to print.</param>
		/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
		void LogDebug(const char* pMessage, bool newLine = true);

		/// <summary>
		/// Logs a message with the severity of Information.
		/// This is the middle severity and should be used for non-fatal events that will cause bugs, but not crashes.
		/// </summary>
		/// <param name="pMessage">(const char*) The message to print.</param>
		/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
		void LogWarning(const char* pMessage, bool newLine = true);

		/// <summary>
		/// Logs a message with the severity of Information. This is the second highest severity. It should be used for anything that will eventually lead to a crash.
		/// </summary>
		/// <param name="pMessage">(const char*) The message to print.</param>
		/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
		void LogSevere(const char* pMessage, bool newLine = true);

		/// <summary>
		/// Logs a message with the severity of Information. This is the highest severity, it should be used when the engine is going to crash.
		/// </summary>
		/// <param name="pMessage">(const char*) The message to print.</param>
		/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
		void LogFatal(const char* pMessage, bool newLine = true);

	private:

		/// <summary>
		/// Retrieved the current time from the system class.
		/// </summary>
		/// <returns>(const char*) The current time in the format: [H:M:S]</returns>
		const char* GetCurrentTime();

		std::ofstream m_outputFile;
		Level m_logLevel;
		bool m_sameLine;
	};
}