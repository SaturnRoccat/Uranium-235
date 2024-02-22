#pragma once
#include "../Utils/rapidProxy.hpp"
#include "../keyGenerator/keyGenerator.hpp"

namespace Uranium
{
	class ComponentParser
	{
	private:
		rapidjson::Document m_Document; // The doc we are parsing

		KeyGenerator m_KeyGen; // The key generator

		const std::array<std::string, 3> m_ComponentTypes = { "item/", "block/", "entity/"}; // The types of components we are parsing
		const std::array<std::string, 1> m_EndData = { "/components/_main.json" }; // The end data of the components
	public:
		ComponentParser();
	private:
		void Run();
	
	};
}