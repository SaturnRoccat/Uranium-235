#include "Logger.hpp"

namespace Uranium
{
	namespace Logs
	{
		// The global logger
		int Logger::pushCount;
		std::ofstream Logger::m_logFile = std::ofstream("Uranium.log", std::ios::out | std::ios::trunc);

		std::vector<std::string>* Logger::m_logBuffer = &Logger::m_logBuffer1;
		std::vector<std::string>* Logger::m_logBufferBack = &Logger::m_logBuffer2;

		std::vector<std::string> Logger::m_logBuffer1 = {};
		std::vector<std::string> Logger::m_logBuffer2 = {};

	}
}