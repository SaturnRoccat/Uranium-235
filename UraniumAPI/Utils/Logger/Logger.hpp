#pragma once
#include <format>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread> // To allow for me to spin a thread to write to the log file
#include <vector>

#ifndef FLUSH_CONSTANT
	#define FLUSH_CONSTANT 128
#endif
#pragma warning(disable : 4996) // Disable the warning about std::ctime being unsafe

namespace Uranium
{
	namespace Logs
	{
		// Helper function to emulate std::format behavior with variadic arguments
		template <typename T>
		void formatHelper(std::ostringstream& oss, const std::string& format, T&& arg) {
			size_t pos = format.find("{}");
			if (pos != std::string::npos) {
				oss << format.substr(0, pos) << arg;
				formatHelper(oss, format.substr(pos + 2), std::forward<T>(arg));
			}
			else {
				oss << format;
			}
		}

		template <typename T, typename... Args>
		void formatHelper(std::ostringstream& oss, const std::string& format, T&& arg, Args&&... args) {
			size_t pos = format.find("{}");
			if (pos != std::string::npos) {
				oss << format.substr(0, pos) << arg;
				formatHelper(oss, format.substr(pos + 2), std::forward<Args>(args)...);
			}
			else {
				oss << format;
			}
		}

		template <typename... Args>
		std::string formatString(const std::string& format, Args&&... args) {
			std::ostringstream oss;
			formatHelper(oss, format, std::forward<Args>(args)...);
			return oss.str();
		}

		enum class LogLevel
		{
			Info,
			Warning,
			Debug,
			Error, 
			NonFatalError
		};

		enum class color
		{
			RESET = 0,
			RED = 31,
			GREEN = 32,
			YELLOW = 33,
			BLUE = 34,
			MAGENTA = 35,
			CYAN = 36,
			WHITE = 37
		};

		constexpr static const char* getColorCode(color c)
		{
			switch (c)
			{
			case color::RED:
				return "\033[31m";
			case color::GREEN:
				return "\033[32m";
			case color::YELLOW:
				return "\033[33m";
			case color::BLUE:
				return "\033[34m";
			case color::MAGENTA:
				return "\033[35m";
			case color::CYAN:
				return "\033[36m";
			case color::WHITE:
				return "\033[37m";
			default:
				return "\033[0m";
			}
		}
		constexpr static const char* LogLevelToColor(LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Info:
				return getColorCode(color::BLUE);
			case LogLevel::Warning:
				return getColorCode(color::YELLOW);
			case LogLevel::Error:
				return getColorCode(color::RED);
			case LogLevel::Debug:
				return getColorCode(color::GREEN);
			case LogLevel::NonFatalError:
				return getColorCode(color::MAGENTA);
			default:
				return getColorCode(color::RESET);
			}
		}
		class Logger
		{
		public:
			static void inline OverWriteLogFile(const std::string& path)
			{
				m_logFile.close();
				m_logFile.open(path, std::ios::out | std::ios::trunc);
			}

			static void LogRaw(LogLevel level, const std::string& message)
			{
				// Flush the buffer if needed
				conditionalFlush();
				// Increment the push count
				pushCount++;
				// Get the current time
				auto time = std::chrono::system_clock::now();
				std::time_t timeNow = std::chrono::system_clock::to_time_t(time);

				// Get the time as a string
				std::string timeString = std::ctime(&timeNow);

				// Remove the newline from the time string
				timeString.pop_back();

				// Get the level as a string
				std::string levelString;
				switch (level)
				{
				case LogLevel::Info:
					levelString = "Info";
					break;
				case LogLevel::Warning:
					levelString = "Warning";
					break;
				case LogLevel::NonFatalError:
					levelString = "NonFatalError";
					break;
				case LogLevel::Error:
					levelString = "Error";
					break;
				case LogLevel::Debug:
					levelString = "Debug";
					break;
				default:
					levelString = "Unknown";
					break;
				}

				// Set the color of the message
				std::cout << LogLevelToColor(level);

				// Format the message
				std::string formattedLog = formatString("[{}] [{}] {}\n", timeString, levelString, message);

				// Add the message to the buffer
				m_logBuffer->push_back(formattedLog);

				// Print the message to the console
				std::cout << formattedLog;

				// Reset the color of the message
				std::cout << getColorCode(color::RESET) << "\n";
			}

			template <typename... Args>
			static void inline LogRaw(LogLevel level, const std::string& message, Args... args)
			{
				std::string formattedMessage = formatString(message, args...);

				// Call the original LogRaw function with the formatted message
				LogRaw(level, formattedMessage);
			}
			template <typename... Args>
			static void inline Error(const std::string& message, Args... args)
			{
				Error(formatString(message, args...));
			}
			template <typename... Args>
			static void inline Warning(const std::string& message, Args... args)
			{
				LogRaw(LogLevel::Warning, message, args...);
			}
			template <typename... Args>
			static void inline Info(const std::string& message, Args... args)
			{
				LogRaw(LogLevel::Info, message, args...);
			}

			template <typename... Args>
			static void inline Debug(const std::string& message, Args... args)
			{
				LogRaw(LogLevel::Debug, message, args...);
			}

			template<typename ... Args>
			static void inline NonFatalError(const std::string& message, Args... args)
			{
				LogRaw(LogLevel::NonFatalError, message, args...);
			}

			static void inline Error(const std::string& message)
			{
				LogRaw(LogLevel::Error, message);
				// flush the buffer 
				flush(true);
				// Throw an exception
				throw std::runtime_error(message);
			}

			static void inline Warning(const std::string& message)
			{
				LogRaw(LogLevel::Warning, message);
			}

			static void inline Info(const std::string& message)
			{
				LogRaw(LogLevel::Info, message);
			}

			static void inline Debug(const std::string& message)
			{
				LogRaw(LogLevel::Debug, message);
			}

			static void inline NonFatalError(const std::string& message)
			{
				LogRaw(LogLevel::NonFatalError, message);
			}

			static void inline flush(bool staticFlush = false)
			{
				std::swap(m_logBuffer, m_logBufferBack);

				if (!staticFlush)
				{
					std::thread t([]() {
						for (auto& message : *m_logBufferBack)
						{
							m_logFile << message << std::endl;
						}
						m_logBufferBack->clear();
						});
					t.detach();
				}
				else
				{
					for (auto& message : *m_logBufferBack)
					{
						m_logFile << message << std::endl;
					}
					m_logBufferBack->clear();
					m_logBufferBack->resize(0);
				}
			}

			static void inline close()
			{
				// We flush twice to make sure that the buffer is empty!
				flush(true);
				flush(true);
				m_logFile.close();
			}
		private:
			static std::ofstream m_logFile;

			// The reason we are doing this is so that when we flush the buffer, We can empty the buffer without having to lock the buffer.
			static std::vector<std::string> *m_logBuffer;
			static std::vector<std::string> *m_logBufferBack;

			static std::vector<std::string> m_logBuffer1;
			static std::vector<std::string> m_logBuffer2;

			static int pushCount;
		private:
			static void  conditionalFlush() {
				if (pushCount == 0)
					return; // Just return if we have not pushed anything to the buffer yet.
				if (pushCount % FLUSH_CONSTANT == 0)
				{
					flush();
				}
			}
		};
	}
}

#pragma warning(default : 4996) // Enable the warning about std::ctime being unsafe