#include "ComponentParser.hpp"
#include <fstream>
#include "../Utils/Logger/Logger.hpp"
#include "../Utils/Timer.hpp"
#include "../memberComponentParsing/memberComponentParser.hpp"
Uranium::ComponentParser::ComponentParser(): m_KeyGen(&this->m_Document) 
{
	std::ifstream file("schemas.json");
	if (!file.is_open()) {
		Logs::Logger::Error("Failed to open schemas.json");
	}

	std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	m_Document.Parse(jsonString.c_str());
	Run();
}

void Uranium::ComponentParser::Run()
{
	Logs::Logger::Info("Locating most up to date keys!");
	std::array<KeyInformation, 3> highestVersionKeys = {};
	{
		ScopedTimer timer("Finding most up to date keys");
		highestVersionKeys = m_KeyGen.findHighestVersionKey<3, 1>(gComponentTypes, gEndData);
	}
	Logs::Logger::Info("Found most up to date keys!");
	for (auto& key : highestVersionKeys) {
		Logs::Logger::Info("Highest Version Key: {}\nLongPrefix Key: {}\nShortPrefix Key: {}", key.highestVersionKey, key.longPrefix, key.shortPrefix);
	}

}
