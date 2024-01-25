#pragma once
#include "../ProjectConfig.hpp"
#include "../Utils/rapidProxy.hpp"
#include <string>

namespace Uranium
{
	namespace Components
	{
		class Component
		{
		public:
			Component(const std::string& name, const std::vector<Experimentals>& reqiredExperiments = {}) : m_name(name), m_reqiredExperiments(reqiredExperiments) {}

			virtual void CompileComponent(RapidProxy::DefaultValueWriter ValueWriter, ProjectSettings* settings) = 0;
		private:
			std::string m_name;
			std::vector<Experimentals> m_reqiredExperiments;
		};
	}
}