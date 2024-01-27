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
			Component(const char* name) : m_name(name) {}

			virtual void CompileComponent(RapidProxy::DefaultValueWriter writer, ProjectSettings* settings) = 0;

			const char* GetName() const { return m_name; }
		private:
			const char* m_name;
		};
	}
}