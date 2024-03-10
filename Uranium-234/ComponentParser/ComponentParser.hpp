#pragma once
#include "../Utils/rapidProxy.hpp"
#include "../keyGenerator/keyGenerator.hpp"

namespace Uranium
{
	const static std::array<std::string, 3> gComponentTypes = { "item/", "block/", "entity/" }; // The types of components we are parsing
	const static std::array<std::string, 3> gParents = { "item", "block", "entity" }; // The types of components we are parsing
	const static std::array<std::string, 1> gEndData = { "/components/_main.json" }; // The end data of the components
	class ComponentParser
	{
	private:
		rapidjson::Document m_Document; // The doc we are parsing

		KeyGenerator m_KeyGen; // The key generator

	public:
		ComponentParser();
	private:
		void Run();
	
	};
}