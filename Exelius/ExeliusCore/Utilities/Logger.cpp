#include "Logger.h"
#include "ApplicationLayer.h"

#include <iostream>
#include <string>

namespace Exelius
{
	/// <summary>
	/// Initializes the logger by seleting an output path and folder name, and logging severity.
	/// </summary>
	/// <param name="level">(Level) Severity to log. Will log anything just as and more severe as this level.</param>
	/// <returns>(bool) True if initialize is a success, false if there was an error. Error will not be logged (since the logger failed to be created).</returns>
	bool Logger::Initialize(Level level, ISystem* pSystem)
	{
		//Set the current logging severity level.
		SetLogLevel(level);

		//Create the Log folder.
		if (!pSystem->SystemCreateDirectory(nullptr, "Logs"))
		{
			//File may already exist if we get here... will need to do further error checking.
			//But if it does already exist, it's not really a big deal.
		}

		//Get the date to append to the end of the name of the log file. "Log_1995_03_27.txt"
		const char* pAppender = pSystem->GetSystemDate();

		//Create the files name, including the logation to place the file (Logs Folder).
		std::string pFilename = "Logs\\Log_";

		//Append the date and the filetype to the filename.
		pFilename += pAppender;
		pFilename += ".txt";

		//Set the counter to 1.
		int counter = 1;

		//while file with name exists already.
		while (pSystem->DoesFileExist(pFilename.c_str()))
		{
			//If the file does exist, append the counter to the end of the filename and check again.

			//Reset the file name to this format "Folder_Name_Date_"
			pFilename = "Logs\\Log_";
			pFilename += pAppender;
			pFilename += "_";
			//Append the counter and filetype to the filename: "Folder_Name_Date_Counter.txt"
			pFilename += std::to_string(counter);
			pFilename += ".txt";

			//Increase the counter.
			++counter;
		}

		//Open the file for writing.
		m_outputFile.open(pFilename);
		if (m_outputFile.bad())
		{
			return false;
		}
		return true;
	}

	/// <summary>
	/// Logs a message with the severity of Information. This is the lowest severity.
	/// </summary>
	/// <param name="pMessage">(const char*) The message to print.</param>
	/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
	void Logger::LogInfo(const char* pMessage, bool newLine)
	{
		if (m_logLevel <= Level::kLevelInfo)
		{
			if (!m_sameLine)
			{
				m_outputFile << GetCurrentTime() << " [INFO] ";
			}

			m_outputFile << pMessage;

			if (newLine)
			{
				m_outputFile << '\n';
				m_sameLine = false;
			}
			else
			{
				m_sameLine = true;
			}
		}
	}

	/// <summary>
	/// Logs a message with the severity of Information. This is the second lowest severity and should be used for debugging purposes.
	/// </summary>
	/// <param name="pMessage">(const char*) The message to print.</param>
	/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
	void Logger::LogDebug(const char* pMessage, bool newLine)
	{
		if (m_logLevel <= Level::kLevelDebug)
		{
			if (!m_sameLine)
			{
				m_outputFile << GetCurrentTime() << " [DBUG] ";
			}

			m_outputFile << pMessage;

			if (newLine)
			{
				m_outputFile << '\n';
				m_sameLine = false;
			}
			else
			{
				m_sameLine = true;
			}
		}
	}

	/// <summary>
	/// Logs a message with the severity of Information.
	/// This is the middle severity and should be used for non-fatal events that will cause bugs, but not crashes.
	/// </summary>
	/// <param name="pMessage">(const char*) The message to print.</param>
	/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
	void Logger::LogWarning(const char* pMessage, bool newLine)
	{
		if (m_logLevel <= Level::kLevelWarning)
		{
			if (!m_sameLine)
			{
				m_outputFile << GetCurrentTime() << " [WARN] ";
			}

			m_outputFile << pMessage;

			if (newLine)
			{
				m_outputFile << '\n';
				m_sameLine = false;
			}
			else
			{
				m_sameLine = true;
			}
		}
	}

	/// <summary>
	/// Logs a message with the severity of Information. This is the second highest severity. It should be used for anything that will eventually lead to a crash.
	/// </summary>
	/// <param name="pMessage">(const char*) The message to print.</param>
	/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
	void Logger::LogSevere(const char* pMessage, bool newLine)
	{
		if (m_logLevel <= Level::kLevelSevere)
		{
			if (!m_sameLine)
			{
				m_outputFile << GetCurrentTime() << " [SVER] ";
			}

			m_outputFile << pMessage;

			if (newLine)
			{
				m_outputFile << '\n';
				m_sameLine = false;
			}
			else
			{
				m_sameLine = true;
			}
		}
	}

	/// <summary>
	/// Logs a message with the severity of Information. This is the highest severity, it should be used when the engine is going to crash.
	/// </summary>
	/// <param name="pMessage">(const char*) The message to print.</param>
	/// <param name="newLine">(bool) Whether or not this message is being printed on a new line.</param>
	void Logger::LogFatal(const char* pMessage, bool newLine)
	{
		if (m_logLevel <= Level::kLevelFatal)
		{
			if (!m_sameLine)
			{
				m_outputFile << GetCurrentTime() << " [FATL] ";
			}

			m_outputFile << pMessage;

			if (newLine)
			{
				m_outputFile << '\n';
				m_sameLine = false;
			}
			else
			{
				m_sameLine = true;
			}
		}
	}

	/// <summary>
	/// Retrieved the current time from the system class.
	/// </summary>
	/// <returns>(const char*) The current time in the format: [H:M:S]</returns>
	const char* Logger::GetCurrentTime()
	{
		return IApplicationLayer::GetInstance()->GetSystem()->GetSystemTime();
	}
}