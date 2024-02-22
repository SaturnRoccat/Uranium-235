#pragma once
#include <chrono>
#include "Logger/Logger.hpp"

namespace Uranium
{
	class ScopedTimer
	{
	public:
		ScopedTimer(const char* name)
			: m_Name(name)
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
		}

		~ScopedTimer()
		{
			auto endTime = std::chrono::high_resolution_clock::now();

			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

			auto duration = end - start;
			double ms = duration * 0.001;

			Logs::Logger::Info("ScopedTimer {} {}ms", m_Name, ms);
		}
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
		const char* m_Name;
	};

	class AverageTimer
	{
	public:
		AverageTimer(const char* name, uint64_t callCount = 0, bool log = false, double* writeLoc = nullptr)
			: m_Name(name), m_callCount(callCount), m_useCout(log), writeLoc(writeLoc)
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
		}

		inline void IncrementCallCount() { m_callCount++; }

		~AverageTimer()
		{
			auto endTime = std::chrono::high_resolution_clock::now();

			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

			auto duration = end - start;
			double ms = duration * 0.001;

			// calculate average
			ms /= m_callCount;

			if (writeLoc)
				*writeLoc = ms;

			if (m_useCout)
				std::cout << std::format("AverageTimer {} {}ms ({} calls)", m_Name, ms, m_callCount) << std::endl;
			else
				Logs::Logger::Info("AverageTimer {} {}ms ({} calls)", m_Name, ms, m_callCount);
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
		const char* m_Name;
		bool m_useCout;
		double* writeLoc;
		uint64_t m_callCount = 0;
	};
}